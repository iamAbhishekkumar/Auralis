
#include "server.hpp"

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>

#include <cstring>
#include <unordered_map>

#include "asp.hpp"
#include "logger.hpp"

void startServer(int port) {
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd < 0) {
        error("Failed to create socket");
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
        return;
    }

    if (listen(listenFd, SOMAXCONN) < 0) {
        error("Listen failed");
        close(listenFd);
        return;
    }

    info("Listening on port", port);

    std::unordered_map<int, int> clients;
    fd_set readFDs;

    while (true) {
        FD_ZERO(&readFDs);
        FD_SET(listenFd, &readFDs);
        int maxFD = listenFd;

        for (const auto& [fd, _] : clients) {
            FD_SET(fd, &readFDs);
            if (fd > maxFD) maxFD = fd;
        }

        if (select(maxFD + 1, &readFDs, nullptr, nullptr, nullptr) < 0) {
            error("Select error");
            continue;
        }

        // New connection
        if (FD_ISSET(listenFd, &readFDs)) {
            int clientFd = accept(listenFd, nullptr, nullptr);
            if (clientFd >= 0) {
                fcntl(clientFd, F_SETFL, O_NONBLOCK);
                clients[clientFd] = clientFd;
                info("Accepted connection. FD:", clientFd);
            }
        }

        // Existing connections
        for (auto it = clients.begin(); it != clients.end();) {
            int fd = it->first;

            if (FD_ISSET(fd, &readFDs)) {
                ValueType value;
                if (!parseRESP(fd, value)) {
                    info("Disconnected. FD:", fd);
                    close(fd);
                    it = clients.erase(it);
                    continue;
                }

                if (value.type != "array") {
                    info("Invalid request");
                    ++it;
                    continue;
                }

                std::string response = marshalString("OK");
                write(fd, response.c_str(), response.size());
            }

            ++it;
        }
    }
}
