project(inplace_transpose)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE func_measuring)

target_include_directories(${PROJECT_NAME} PRIVATE src)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})