project(custom_optional_test)

add_executable(${PROJECT_NAME} src/custom_optional_test.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
    feature_exploring::custom_optional
    gtest_wrapper
)
enable_project_tools(${PROJECT_NAME})
