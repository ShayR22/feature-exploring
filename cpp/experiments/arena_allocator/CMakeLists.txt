project(arena_allocator)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE feature_exploring::arena_allocator)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})