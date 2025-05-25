#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define BOLD "\033[1m"

void printLogo();
void printServerInfo(const std::string& mode, int port);
void info(const std::string& msg, int val = -1);
void error(const std::string& msg);

#endif