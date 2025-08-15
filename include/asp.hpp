#ifndef RESP_HPP
#define RESP_HPP

#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

struct ValueType {
    std::string type;              // "array", "bulk", "string", "null", "error"
    std::string str;               // for "string" and "error"
    std::string bulk;              // for bulk values
    std::vector<ValueType> array;  // for arrays

    // Marshal to RESP string
    std::string Marshal() const;

   private:
    std::string marshalString() const;
    std::string marshalBulk() const;
    std::string marshalArray() const;
    std::string marshalError() const;
    std::string marshalNull() const;
};

// ----- Factory helpers (Option A) -----
inline ValueType makeBulk(const std::string &s) {
    ValueType v;
    v.type = "bulk";
    v.bulk = s;
    return v;
}

inline ValueType makeString(const std::string &s) {
    ValueType v;
    v.type = "string";
    v.str = s;
    return v;
}

inline ValueType makeError(const std::string &s) {
    ValueType v;
    v.type = "error";
    v.str = s;
    return v;
}

inline ValueType makeNull() {
    ValueType v;
    v.type = "null";
    return v;
}

inline ValueType makeArray(const std::vector<ValueType> &elems = {}) {
    ValueType v;
    v.type = "array";
    v.array = elems;
    return v;
}

// parseRESP forward decl is in resp.cpp
bool parseRESP(int fd, ValueType &out);

#endif  // RESP_HPP
