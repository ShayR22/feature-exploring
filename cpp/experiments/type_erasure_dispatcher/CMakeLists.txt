project(type_erasure_dispatcher)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE feature_exploring::type_erasure_dispatcher)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})