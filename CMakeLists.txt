cmake_minimum_required(VERSION 3.29)
project(NEXIS_DB)

include_directories(include)

set(NEXIS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB SOURCES ${NEXIS_SRC_DIR}/*.cpp)

add_executable(nexis ${SOURCES})

# target_link_libraries()