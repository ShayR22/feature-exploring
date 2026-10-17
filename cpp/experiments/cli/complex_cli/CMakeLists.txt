project(complex)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src ../cli11)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})