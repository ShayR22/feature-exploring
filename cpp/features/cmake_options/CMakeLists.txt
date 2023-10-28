
cmake_minimum_required(VERSION 3.22)

project(example)

set(SOURCES
    main.cpp
)

# add_subdirectory("cmake_options")
include("cmake_options/options.cmake")

add_executable(example ${SOURCES})


