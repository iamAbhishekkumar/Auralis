// src/logger.cpp
#include "logger.hpp"

#include <iostream>

void printLogo() {
    std::string logo[] = {CYAN "  ___                      _  _       ",
                          " / _ \\                    | |(_)     ",
                          "/ /_\\ \\ _   _  _ __  __ _ | | _  ___  ",
                          "|  _  || | | || '__|/ _` || || |/ __| ",
                          "| | | || |_| || |  | (_| || || |\\__ \\",
                          "\\_| |_/ \\__,_||_|   \\__,_||_||_||___/",
                          BOLD "\n       A U R A L I S   D B" RESET};
    for (const auto& l : logo) std::cout << l << "\n";
}

void printServerInfo(const std::string& mode, int port) {
    std::cout << "╭────────────────────────────╮\n";
    std::cout << "│ Mode : " << mode << "\n";
    std::cout << "│ Port : " << port << "\n";
    std::cout << "╰────────────────────────────╯\n";
    std::cout << "[OK] Ready to accept connections\n";
}
