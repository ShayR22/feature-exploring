project(base_file_name)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/dir_x/dir_y/print_x.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})