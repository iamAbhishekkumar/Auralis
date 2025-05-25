// src/resp.cpp
#include "asp.hpp"
#include <sstream>

bool readLine(int fd, std::string& line) {
    char c;
    line.clear();
    while (read(fd, &c, 1) == 1) {
        line += c;
        if (line.size() >= 2 && line[line.size() - 2] == '\r') {
            line.resize(line.size() - 2);
            return true;
        }
    }
    return false;
}

bool parseInteger(int fd, int& result) {
    std::string line;
    if (!readLine(fd, line)) return false;
    result = std::stoi(line);
    return true;
}

bool parseRESP(int fd, ValueType& out) {
    char typeChar;
    if (read(fd, &typeChar, 1) <= 0) return false;

    if (typeChar == '*') {
        int len;
        if (!parseInteger(fd, len)) return false;
        out.type = "array";
        out.array.reserve(len);
        for (int i = 0; i < len; ++i) {
            ValueType elem;
            if (!parseRESP(fd, elem)) return false;
            out.array.push_back(elem);
        }
        return true;
    }

    if (typeChar == '$') {
        int len;
        if (!parseInteger(fd, len)) return false;
        std::string bulk(len, '\0');
        read(fd, bulk.data(), len);
        std::string _crlf;
        readLine(fd, _crlf); // Consume CRLF
        out.type = "bulk";
        out.str = bulk;
        return true;
    }

    return false;
}

std::string marshalString(const std::string& s) {
    return "+" + s + "\r\n";
}
