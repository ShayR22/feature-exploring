project(gtest_wrapper)
find_package(GTest CONFIG REQUIRED)

add_library(gtest_wrapper STATIC
    srcs/gtest_wrapper_reporter.cpp
    srcs/gtest_wrapper_runner.cpp
    srcs/gtest_wrapper_runtime_configuration.cpp
)
target_include_directories(gtest_wrapper PUBLIC include)
target_link_libraries(gtest_wrapper PUBLIC GTest::gmock)
enable_project_tools(${PROJECT_NAME})
