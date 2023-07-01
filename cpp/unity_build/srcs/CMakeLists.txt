
cmake_minimum_required(VERSION 3.22)
project(custom_classes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O2")

file(GLOB_RECURSE SRCS ./*.cpp)
add_library(${PROJECT_NAME} STATIC ${SRCS})
