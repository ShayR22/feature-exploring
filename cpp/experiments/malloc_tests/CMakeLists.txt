project(malloc_tests)

add_executable(${PROJECT_NAME}
    src/allocate.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE func_measuring)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})