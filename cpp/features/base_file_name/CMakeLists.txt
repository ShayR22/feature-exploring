project(base_file_name)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
    srcs/dir_x/dir_y/print_x.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE srcs)

include(${CMAKE_INSTALL_BIN_HELPER})
include(${CMAKE_TOOLS_PATH})