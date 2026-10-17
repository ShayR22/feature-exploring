project(buf_to_string)

add_executable(${PROJECT_NAME}
    srcs/buf_to_string.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE func_measuring)

target_include_directories(${PROJECT_NAME} PRIVATE srcs)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})