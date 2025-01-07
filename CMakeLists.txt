cmake_minimum_required(VERSION 3.10)
project(TA)

set(CMAKE_CXX_COMPILER "g++")

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(TA ${SOURCE_FILES})