# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (VariantSocket)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

SET(PROJECT_SOURCES
    uds/uds_data.cpp
    uds/uds_webserver.cpp
    uds/uds_client.cpp
)

include_directories("uds")

# add_executable(variant_socket ${PROJECT_SOURCES})

add_executable(server_main server_main.cpp ${PROJECT_SOURCES})
add_executable(client_main client_main.cpp ${PROJECT_SOURCES})

