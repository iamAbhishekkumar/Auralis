#ifndef AOF_HPP
#define AOF_HPP

#include <fstream>
#include <functional>
#include <string>

#include "asp.hpp"

class Aof {
   public:
    Aof(const std::string& path);
    ~Aof();

    void write(const ValueType& value);
    void replay(const std::function<void(const ValueType&)>& fn);

   private:
    std::ofstream file;
    std::string path;
};

// Global pointer declaration (extern)
extern Aof* gAof;

#endif
