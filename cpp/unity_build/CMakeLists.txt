
cmake_minimum_required(VERSION 3.22)
project(custom_classes_unity_build)

SET(SRCS unity_build.cpp)
add_library(${PROJECT_NAME} STATIC ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
