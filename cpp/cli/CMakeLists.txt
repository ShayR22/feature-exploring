cmake_minimum_required(VERSION 3.10)
project(cli_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O2")

set(SRC_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})
add_executable(${PROJECT_NAME}2 main2.cpp)
