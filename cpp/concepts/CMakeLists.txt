cmake_minimum_required(VERSION 3.10)
project(concepts_main)

find_library(SPDLOG
            NAMES libspdlog.a
            PATHS "${CMAKE_SOURCE_DIR}/libs"

)

message(NOTICE "path: ${CMAKE_SOURCE_DIR}/libs/")
message(STATUS "SPDLOG: ${SPDLOG}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O2")

set(SRC_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    "headers"
)

target_link_libraries(${PROJECT_NAME}
    ${SPDLOG}
)