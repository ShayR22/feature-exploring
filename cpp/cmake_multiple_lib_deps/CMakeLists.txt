cmake_minimum_required(VERSION 3.22)
project("libs_deps")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the install destination to the build directory
set(INSTALL_DIR "${CMAKE_BINARY_DIR}/install")


add_subdirectory("kita_alef")
add_subdirectory("kita_bet")
add_subdirectory("my_main")