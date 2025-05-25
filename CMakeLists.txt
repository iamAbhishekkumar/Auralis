cmake_minimum_required(VERSION 3.14)
project(auralis)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(auralis-server
    src/main.cpp src/server.cpp src/asp.cpp src/logger.cpp
)

add_executable(auralis-cli
    cli/main.cpp
)
