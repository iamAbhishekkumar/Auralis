#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "aof.hpp"
#include "asp.hpp"
#include "asp.hpp"  // ValueType and makeBulk/makeArray helpers
#include "store.hpp"

using HandlerFunc = std::function<ValueType(const std::vector<ValueType>&)>;

// Command handlers
ValueType ping(const std::vector<ValueType>& args);
ValueType set(const std::vector<ValueType>& args);
ValueType get(const std::vector<ValueType>& args);
ValueType hset(const std::vector<ValueType>& args);
ValueType hget(const std::vector<ValueType>& args);
ValueType hgetall(const std::vector<ValueType>& args);

// Return map of command string → handler
std::unordered_map<std::string, HandlerFunc> getHandlers();

#endif  // COMMANDS_HPP
