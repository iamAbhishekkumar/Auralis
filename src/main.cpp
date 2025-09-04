#include "aof.hpp"
#include "logger.hpp"
#include "server.hpp"

int main() {
    const int port = 7379;
    const std::string mode = "Standalone";
    const std::string aofPath = "dump.aof";  // specify your AOF file

    // Initialize global AOF
    gAof = new Aof(aofPath);

    printLogo();
    printServerInfo(mode, port);

    // Start server with port and AOF
    startServer(port, aofPath);

    // Cleanup AOF on exit
    delete gAof;
    gAof = nullptr;

    return 0;
}
