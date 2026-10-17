project(btree_iterator)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE srcs)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})