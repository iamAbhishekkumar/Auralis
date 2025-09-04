#include "store.hpp"

std::unordered_map<std::string, std::string> gStringStore;
std::mutex gStringStoreMutex;

std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
    gHashStore;
std::mutex gHashStoreMutex;
