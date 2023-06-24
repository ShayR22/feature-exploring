
cmake_minimum_required(VERSION 3.22)
project(kita_alef)

SET(SRCS add.cpp sub.cpp)
add_library(kita_alef STATIC ${SRCS})
target_include_directories(kita_alef PUBLIC ${PROJECT_BASE_DIR})