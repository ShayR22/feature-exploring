cmake_minimum_required(VERSION 3.22)

project(kita_bet)

SET(HEADERS
    include/kita_bet/mul.hpp
)

SET(SRCS
    srcs/mul.cpp
)

add_library(kita_bet STATIC ${SRCS})
target_include_directories(kita_bet PUBLIC include)
target_link_libraries(kita_bet PUBLIC kita_alef)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${PROJECT_NAME}"
)