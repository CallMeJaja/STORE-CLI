cmake_minimum_required(VERSION 3.10)
project(TA)

set(SOURCE_FILES)
aux_source_directory(src SOURCE_FILES)


include_directories(include)
set(CMAKE_CXX_COMPILER "g++")

add_executable(TA ${SOURCE_FILES})

