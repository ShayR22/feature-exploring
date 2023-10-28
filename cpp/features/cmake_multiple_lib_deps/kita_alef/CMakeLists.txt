
cmake_minimum_required(VERSION 3.22)
project(kita_alef)

SET(SRCS
    srcs/add.cpp
    srcs/sub.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC include)

install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${INSTALL_DIR}/lib/${PROJECT_NAME}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/
    DESTINATION ${INSTALL_DIR}/lib/${PROJECT_NAME}/include
    FILES_MATCHING PATTERN "*.h*"
)