project(type_erasure)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../features/type_erasure")

target_link_libraries(${PROJECT_NAME} PRIVATE func_measuring)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})