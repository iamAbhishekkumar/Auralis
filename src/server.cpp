// src/server.cpp
#include "server.hpp"

#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/select.h>
#include <unistd.h>

#include <atomic>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>

#include "aof.hpp"
#include "asp.hpp"
#include "commands.hpp"
#include "logger.hpp"

// flag for graceful shutdown
static std::atomic<bool> gRunning{true};

static void handleSignal(int /*signum*/) { gRunning.store(false); }

// Start the server on the given port
void startServer(int port, const std::string& aofPath) {
    // install signal handlers for graceful shutdown
    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handleSignal;
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);

    bool createdAofHere = false;

    // Initialize AOF only if not already created elsewhere
    if (gAof == nullptr) {
        try {
            gAof = new Aof(aofPath);
            createdAofHere = true;
            info("AOF initialized at", aofPath);
        } catch (const std::exception& e) {
            error("Failed to initialize AOF:", e.what());
            // we can continue without AOF, but warn the user
            gAof = nullptr;
        }
    } else {
        info("Using existing AOF instance");
    }

    // Prepare handlers map BEFORE replay so we can dispatch commands
    auto handlers = getHandlers();

    // Replay persisted AOF (if available)
    if (gAof) {
        info("Replaying AOF...");
        // Temporarily disable AOF writes while replaying to avoid duplicate
        // writes. We'll set gAof to nullptr while invoking handlers, then
        // restore it.
        Aof* savedAof = gAof;
        gAof = nullptr;

        savedAof->replay([&handlers](const ValueType& cmd) {
            if (cmd.type != "array" || cmd.array.empty()) return;

            std::string cmdName = cmd.array[0].bulk;
            std::transform(cmdName.begin(), cmdName.end(), cmdName.begin(),
                           ::toupper);

            auto hIt = handlers.find(cmdName);
            if (hIt != handlers.end()) {
                std::vector<ValueType> args(cmd.array.begin() + 1,
                                            cmd.array.end());
                // Call handler to apply command to in-memory stores
                hIt->second(args);
            } else {
                // unknown command in aof — just log
                info("Unknown command in AOF during replay:", cmdName);
            }
        });

        // restore aof pointer so subsequent commands will be appended
        gAof = savedAof;
        info("AOF replay complete");
    }

    // Create listening socket
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd < 0) {
        error("Failed to create socket");
        if (createdAofHere && gAof) {
            delete gAof;
            gAof = nullptr;
        }
        return;
    }

    fcntl(listenFd, F_SETFL, O_NONBLOCK);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenFd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        error("Bind failed");
        close(listenFd);
        if (createdAofHere && gAof) {
            delete gAof;
            gAof = nullptr;
        }
        return;
    }

    if (listen(listenFd, SOMAXCONN) < 0) {
        error("Listen failed");
        close(listenFd);
        if (createdAofHere && gAof) {
            delete gAof;
            gAof = nullptr;
        }
        return;
    }

    info("Listening on port", port);

    std::unordered_map<int, int> clients;
    fd_set readFDs;

    while (gRunning.load()) {
        FD_ZERO(&readFDs);
        FD_SET(listenFd, &readFDs);
        int maxFD = listenFd;

        for (const auto& [fd, _] : clients) {
            FD_SET(fd, &readFDs);
            if (fd > maxFD) maxFD = fd;
        }

        // timeout: make select wake periodically so shutdown can be checked
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int ready = select(maxFD + 1, &readFDs, nullptr, nullptr, &tv);
        if (ready < 0) {
            if (errno == EINTR) continue;
            error("Select error:", std::strerror(errno));
            continue;
        }

        // Accept new connections
        if (FD_ISSET(listenFd, &readFDs)) {
            int clientFd = accept(listenFd, nullptr, nullptr);
            if (clientFd >= 0) {
                fcntl(clientFd, F_SETFL, O_NONBLOCK);
                clients[clientFd] = clientFd;
                info("Accepted connection. FD:", clientFd);
            }
        }

        // Handle existing clients
        for (auto it = clients.begin(); it != clients.end();) {
            int fd = it->first;

            if (FD_ISSET(fd, &readFDs)) {
                ValueType request;
                if (!parseRESP(fd, request)) {
                    info("Disconnected. FD:", fd);
                    close(fd);
                    it = clients.erase(it);
                    continue;
                }

                if (request.type != "array" || request.array.empty()) {
                    info("Invalid request");
                    ++it;
                    continue;
                }

                std::string cmdName = request.array[0].bulk;
                std::transform(cmdName.begin(), cmdName.end(), cmdName.begin(),
                               ::toupper);

                ValueType response;
                auto itHandler = handlers.find(cmdName);
                if (itHandler != handlers.end()) {
                    // Call command handler with remaining args
                    std::vector<ValueType> args(request.array.begin() + 1,
                                                request.array.end());
                    response = itHandler->second(args);
                } else {
                    response = ValueType{
                        "error", "ERR unknown command '" + cmdName + "'"};
                }

                std::string out = response.Marshal();
                ssize_t w = ::write(fd, out.c_str(), out.size());
                if (w < 0) {
                    info("Write error to FD", fd, ":", std::strerror(errno));
                }
            }

            ++it;
        }
    }  // end main loop

    info("Shutting down server...");

    // close clients
    for (const auto& [fd, _] : clients) close(fd);
    clients.clear();

    close(listenFd);

    // Delete AOF only if we created it here. If main() created it, main should
    // delete it.
    if (createdAofHere && gAof) {
        delete gAof;
        gAof = nullptr;
    }

    info("Server exited cleanly");
}
