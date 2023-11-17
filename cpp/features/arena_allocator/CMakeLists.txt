project(arena_allocator)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
    srcs/arena_allocator.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE srcs)

include(${CMAKE_INSTALL_BIN_HELPER})
include(${CMAKE_TOOLS_PATH})