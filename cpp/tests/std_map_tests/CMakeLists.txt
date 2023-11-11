project(std_map_tests)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
)

add_executable(std_map_tests_map_generator
    srcs/map_generator.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE func_measuring)

target_include_directories(${PROJECT_NAME} PRIVATE srcs)

include(${CMAKE_INSTALL_BIN_HELPER})
include(${CMAKE_TOOLS_PATH})