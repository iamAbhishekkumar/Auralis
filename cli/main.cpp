// cli/main.cpp
#include <netinet/in.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7389);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    connect(sock, (sockaddr*)&addr, sizeof(addr));
    std::string cmd = "*1\r\n$4\r\nPING\r\n";
    write(sock, cmd.c_str(), cmd.size());

    char buffer[1024];
    int n = read(sock, buffer, sizeof(buffer));
    std::cout << std::string(buffer, n);

    close(sock);
}
