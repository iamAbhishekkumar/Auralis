// include/resp.hpp

#ifndef ASP_HPP
#define ASP_HPP

void startServer(int port);

#include <unistd.h>

#include <string>
#include <vector>

struct ValueType {
    std::string type;
    std::string str;
    std::vector<ValueType> array;
};

bool parseRESP(int fd, ValueType& out);
std::string marshalString(const std::string& s);

#endif