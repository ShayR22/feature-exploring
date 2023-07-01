cmake_minimum_required(VERSION 3.22)
project("type_erasure")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O2")

set(PROJECT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("unique_ptr_simple_type_erasure")
add_subdirectory("unique_ptr_type_erasure")
add_subdirectory("small_buffer_optimization_type_erasure")
