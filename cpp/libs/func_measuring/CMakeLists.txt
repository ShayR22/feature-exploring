project(func_measuring)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

install_project_library(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})