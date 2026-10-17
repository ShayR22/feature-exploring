project(atmomic_vs_lock)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})