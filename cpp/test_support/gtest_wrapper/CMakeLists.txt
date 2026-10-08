cmake_minimum_required(VERSION 3.22)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(gtest_wrapper STATIC
    srcs/gtest_wrapper_reporter.cpp
    srcs/gtest_wrapper_runner.cpp
    srcs/gtest_wrapper_runtime_configuration.cpp
)
target_include_directories(gtest_wrapper PUBLIC include)
target_link_libraries(gtest_wrapper PUBLIC GTest::gmock)
if(USE_IWYU)
    set_property(TARGET gtest_wrapper PROPERTY CXX_INCLUDE_WHAT_YOU_USE "${IWYU_PATH_AND_OPTIONS}")
endif()
if(USE_CLANG_TIDY)
    set_target_properties(gtest_wrapper PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_PATH_WITH_OPTIONS}")
endif()
