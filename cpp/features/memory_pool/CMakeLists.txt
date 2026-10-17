project(memory_pool)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE feature_exploring::memory_pool)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})