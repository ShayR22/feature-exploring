project(type_erasure)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE feature_exploring::type_erasure)

target_link_libraries(${PROJECT_NAME} PRIVATE func_measuring)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})