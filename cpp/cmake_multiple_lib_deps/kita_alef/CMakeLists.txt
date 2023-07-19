
cmake_minimum_required(VERSION 3.22)
project(kita_alef)

SET(HEADERS
    include/kita_alef/add.hpp
    include/kita_alef/sub.hpp
)

SET(SRCS
    srcs/add.cpp
    srcs/sub.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC include)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${PROJECT_NAME}"
)
