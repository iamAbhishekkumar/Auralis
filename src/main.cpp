#include "logger.hpp"
#include "server.hpp"

int main() {
    const int port = 63798;
    const std::string mode = "Standalone";

    printLogo();
    printServerInfo(mode, port);
    startServer(port);

    return 0;
}