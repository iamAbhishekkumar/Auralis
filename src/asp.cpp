#include "asp.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

// --- Utility: read a line ending with CRLF ---
bool readLine(int fd, std::string& line) {
    char c;
    line.clear();
    while (read(fd, &c, 1) == 1) {
        line += c;
        if (line.size() >= 2 && line[line.size() - 2] == '\r' &&
            line.back() == '\n') {
            line.resize(line.size() - 2);  // strip CRLF
            return true;
        }
    }
    return false;
}

// --- Utility: parse integer from RESP ---
bool parseInteger(int fd, int& result) {
    std::string line;
    if (!readLine(fd, line)) return false;
    try {
        result = std::stoi(line);
    } catch (...) {
        return false;
    }
    return true;
}

// --- RESP Parser ---
bool parseRESP(int fd, ValueType& out) {
    char typeChar;
    if (read(fd, &typeChar, 1) <= 0) return false;

    switch (typeChar) {
        case '*': {  // Array
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
        case '$': {  // Bulk string
            int len;
            if (!parseInteger(fd, len)) return false;
            if (len == -1) {  // Null bulk
                out.type = "null";
                return true;
            }
            std::string bulk(len, '\0');
            if (read(fd, bulk.data(), len) != len) return false;
            std::string _crlf;
            readLine(fd, _crlf);  // consume CRLF
            out.type = "bulk";
            out.bulk = bulk;
            return true;
        }
        case '+': {  // Simple string
            std::string line;
            if (!readLine(fd, line)) return false;
            out.type = "string";
            out.str = line;
            return true;
        }
        case '-': {  // Error
            std::string line;
            if (!readLine(fd, line)) return false;
            out.type = "error";
            out.str = line;
            return true;
        }
        case ':': {  // Integer (encoded as string here)
            std::string line;
            if (!readLine(fd, line)) return false;
            out.type = "string";
            out.str = line;
            return true;
        }
        default:
            return false;
    }
}

// --- Marshal functions ---
std::string ValueType::Marshal() const {
    if (type == "array") {
        return marshalArray();
    } else if (type == "bulk") {
        return marshalBulk();
    } else if (type == "string") {
        return marshalString();
    } else if (type == "null") {
        return marshalNull();
    } else if (type == "error") {
        return marshalError();
    }
    return "";
}

std::string ValueType::marshalString() const { return "+" + str + "\r\n"; }

std::string ValueType::marshalBulk() const {
    std::ostringstream oss;
    oss << "$" << bulk.size() << "\r\n" << bulk << "\r\n";
    return oss.str();
}

std::string ValueType::marshalArray() const {
    std::ostringstream oss;
    oss << "*" << array.size() << "\r\n";
    for (const auto& elem : array) {
        oss << elem.Marshal();
    }
    return oss.str();
}

std::string ValueType::marshalError() const { return "-" + str + "\r\n"; }

std::string ValueType::marshalNull() const { return "$-1\r\n"; }
