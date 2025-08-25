#include "commands.hpp"

#include <algorithm>
#include <iostream>

// Ensure store globals are provided by store.cpp (extern in store.hpp)
extern std::unordered_map<std::string, std::string> gStringStore;
extern std::mutex gStringStoreMutex;

extern std::unordered_map<std::string,
                          std::unordered_map<std::string, std::string>>
    gHashStore;
extern std::mutex gHashStoreMutex;

// gAof declared extern in aof.hpp and defined in aof.cpp
extern Aof* gAof;

// ---------------- PING ----------------
ValueType ping(const std::vector<ValueType>& args) {
    if (args.empty()) return makeString("PONG");
    // If caller passed an argument, prefer bulk content if present
    if (args[0].type == "bulk") return makeString(args[0].bulk);
    return makeString(args[0].str);
}

// ---------------- SET ----------------
ValueType set(const std::vector<ValueType>& args) {
    if (args.size() != 2)
        return makeError("ERR wrong number of arguments for 'set'");

    std::string key = args[0].bulk;
    std::string value = args[1].bulk;

    {
        std::lock_guard<std::mutex> lock(gStringStoreMutex);
        gStringStore[key] = value;
    }

    // Persist to AOF using factory helpers (safe)
    if (gAof) {
        ValueType cmd = makeArray();
        cmd.array.push_back(makeBulk("SET"));
        cmd.array.push_back(makeBulk(key));
        cmd.array.push_back(makeBulk(value));
        gAof->write(cmd);
    }

    return makeString("OK");
}

// ---------------- GET ----------------
ValueType get(const std::vector<ValueType>& args) {
    if (args.size() != 1)
        return makeError("ERR wrong number of arguments for 'get'");

    std::string key = args[0].bulk;

    std::lock_guard<std::mutex> lock(gStringStoreMutex);
    auto it = gStringStore.find(key);
    if (it == gStringStore.end()) return makeNull();

    return makeBulk(it->second);
}

// ---------------- HSET ----------------
ValueType hset(const std::vector<ValueType>& args) {
    if (args.size() != 3)
        return makeError("ERR wrong number of arguments for 'hset'");

    std::string hash = args[0].bulk;
    std::string field = args[1].bulk;
    std::string value = args[2].bulk;

    {
        std::lock_guard<std::mutex> lock(gHashStoreMutex);
        gHashStore[hash][field] = value;
    }

    // Persist to AOF safely
    if (gAof) {
        ValueType cmd = makeArray();
        cmd.array.push_back(makeBulk("HSET"));
        cmd.array.push_back(makeBulk(hash));
        cmd.array.push_back(makeBulk(field));
        cmd.array.push_back(makeBulk(value));
        gAof->write(cmd);
    }

    return makeString("OK");
}

// ---------------- HGET ----------------
ValueType hget(const std::vector<ValueType>& args) {
    if (args.size() != 2)
        return makeError("ERR wrong number of arguments for 'hget'");

    std::string hash = args[0].bulk;
    std::string field = args[1].bulk;

    std::lock_guard<std::mutex> lock(gHashStoreMutex);
    auto itHash = gHashStore.find(hash);
    if (itHash == gHashStore.end()) return makeNull();

    auto itField = itHash->second.find(field);
    if (itField == itHash->second.end()) return makeNull();

    return makeBulk(itField->second);
}

// ---------------- HGETALL ----------------
ValueType hgetall(const std::vector<ValueType>& args) {
    if (args.size() != 1)
        return makeError("ERR wrong number of arguments for 'hgetall'");

    std::string hash = args[0].bulk;

    std::lock_guard<std::mutex> lock(gHashStoreMutex);
    auto itHash = gHashStore.find(hash);
    if (itHash == gHashStore.end()) return makeNull();

    ValueType result = makeArray();
    for (const auto& kv : itHash->second) {
        result.array.push_back(makeBulk(kv.first));
        result.array.push_back(makeBulk(kv.second));
    }
    return result;
}

// ---------------- Handlers ----------------
std::unordered_map<std::string, HandlerFunc> getHandlers() {
    return {
        {"PING", ping}, {"SET", set},   {"GET", get},
        {"HSET", hset}, {"HGET", hget}, {"HGETALL", hgetall},
    };
}
