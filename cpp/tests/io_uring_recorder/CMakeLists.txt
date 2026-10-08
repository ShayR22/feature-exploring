add_executable(io_uring_recorder_test srcs/main.cpp srcs/validation.cpp srcs/benchmark.cpp)
target_link_libraries(io_uring_recorder_test PRIVATE io_uring_recorder::io_uring_recorder gtest_wrapper)
# The unchanged wrapper macros use GNU named variadic arguments.
target_compile_options(io_uring_recorder_test PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-variadic-macros)
if(USE_IWYU)
    set_property(TARGET io_uring_recorder_test PROPERTY CXX_INCLUDE_WHAT_YOU_USE "${IWYU_PATH_AND_OPTIONS}")
endif()
if(USE_CLANG_TIDY)
    set_target_properties(io_uring_recorder_test PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_PATH_WITH_OPTIONS}")
endif()
install(TARGETS io_uring_recorder_test
    RUNTIME DESTINATION ${INSTALL_DIR}/bin/io_uring_recorder_test
    OPTIONAL
)
