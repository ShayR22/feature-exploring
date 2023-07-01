
cmake_minimum_required(VERSION 3.22)
project(custom_classes)

file(GLOB_RECURSE SRCS ./*.cpp)
add_library(${PROJECT_NAME} STATIC ${SRCS})
