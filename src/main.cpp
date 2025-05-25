#include "logger.hpp"
#include "server.hpp"

int main() {
    const int port = 7389;
    const std::string mode = "Standalone";

    printLogo();
    printServerInfo(mode, port);
    startServer(port);

    return 0;
}