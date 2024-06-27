project(memory_pool)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE srcs)

include(${CMAKE_INSTALL_BIN_HELPER})
include(${CMAKE_TOOLS_PATH})