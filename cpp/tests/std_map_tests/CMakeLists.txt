project(std_map_tests)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
)

add_executable(std_map_tests_map_generator
    srcs/map_generator.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE func_measuring)

target_include_directories(${PROJECT_NAME} PRIVATE srcs)

install_project_binary(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})
enable_project_tools(std_map_tests_map_generator)
# This utility has a distinct target name from the experiment's PROJECT_NAME.
install(TARGETS std_map_tests_map_generator
    RUNTIME DESTINATION ${INSTALL_DIR}/bin/std_map_tests_map_generator
)
