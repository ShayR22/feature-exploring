cmake_minimum_required(VERSION 3.22)
project(kita_bet)
SET(SRCS mul.cpp)

add_library(kita_bet STATIC ${SRCS})
target_link_libraries(kita_bet PUBLIC kita_alef)
target_include_directories(kita_bet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

