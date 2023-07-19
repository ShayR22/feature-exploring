cmake_minimum_required(VERSION 3.22)
project("libs_deps")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory("kita_alef")
add_subdirectory("kita_bet")
add_subdirectory("my_main")