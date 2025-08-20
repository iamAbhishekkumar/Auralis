#include "aof.hpp"

#include <iostream>
#include <stdexcept>

// Define global pointer exactly once
Aof* gAof = nullptr;

Aof::Aof(const std::string& p) : path(p) {
    file.open(path, std::ios::in | std::ios::out | std::ios::app);
    if (!file.is_open()) throw std::runtime_error("Failed to open AOF file");
}

Aof::~Aof() {
    if (file.is_open()) file.close();
}

void Aof::write(const ValueType& value) {
    if (!file.is_open()) return;
    std::string data = value.Marshal();
    file.write(data.c_str(), data.size());
    file.flush();
}

// Simple replay: call fn for each value in AOF
void Aof::replay(const std::function<void(const ValueType&)>& fn) {
    // TODO: implement RESP parsing from file for replay
}
