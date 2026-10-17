project(base_file_name)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
    srcs/dir_x/dir_y/print_x.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE srcs)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})