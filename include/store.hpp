#ifndef STORE_HPP
#define STORE_HPP

#include <mutex>
#include <string>
#include <unordered_map>

extern std::unordered_map<std::string, std::string> gStringStore;
extern std::mutex gStringStoreMutex;

extern std::unordered_map<std::string,
                          std::unordered_map<std::string, std::string>>
    gHashStore;
extern std::mutex gHashStoreMutex;

#endif
