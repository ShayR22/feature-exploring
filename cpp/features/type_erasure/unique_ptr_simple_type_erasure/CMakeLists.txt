
cmake_minimum_required(VERSION 3.22)
project(simple_unique_ptr_type_erasure)

include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BASE_DIR})
add_executable(${PROJECT_NAME} main.cpp)
