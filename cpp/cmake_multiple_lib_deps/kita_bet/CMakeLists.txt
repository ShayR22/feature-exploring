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

install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${INSTALL_DIR}/lib/${PROJECT_NAME}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/
    DESTINATION ${INSTALL_DIR}/lib/${PROJECT_NAME}/include
    FILES_MATCHING PATTERN "*.h*"
)