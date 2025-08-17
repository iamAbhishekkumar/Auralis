#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <iostream>
#include <sstream>
#include <string>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define BOLD "\033[1m"

void printLogo();
void printServerInfo(const std::string& mode, int port);

// Variadic info logger: concatenates arguments and prints to stdout
template <typename... Args>
inline void info(const Args&... args) {
    std::ostringstream oss;
    // fold expression: writes each arg into the stream
    (oss << ... << args);
    std::cout << CYAN << "[INFO] " << RESET << oss.str() << "\n";
}

// Variadic error logger: concatenates arguments and prints to stderr
template <typename... Args>
inline void error(const Args&... args) {
    std::ostringstream oss;
    (oss << ... << args);
    std::cerr << RED << "[ERROR] " << RESET << oss.str() << "\n";
}

#endif