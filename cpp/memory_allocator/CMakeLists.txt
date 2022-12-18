cmake_minimum_required(VERSION 3.10)
project(tester)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O2")

set(SRC_FILES main.cpp)

add_executable(benchmarking ${SRC_FILES})
