cmake_minimum_required(VERSION 3.10)
project(TA)

set(CMAKE_CXX_COMPILER "g++")
message(STATUS  CMAKE_CXX_STANDARD)


include_directories(include) 
include_directories(libs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB BCRYPT_SOURCES libs/bcrypt/*.cpp) 

add_executable(TA ${SOURCE_FILES})

add_library(bcrypt STATIC ${BCRYPT_SOURCES})
target_link_libraries(TA PRIVATE bcrypt)