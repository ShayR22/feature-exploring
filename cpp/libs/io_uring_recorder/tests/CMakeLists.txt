project(io_uring_recorder_test)
add_executable(io_uring_recorder_test src/main.cpp src/validation.cpp src/benchmark.cpp)
target_link_libraries(io_uring_recorder_test PRIVATE io_uring_recorder::io_uring_recorder gtest_wrapper)
# The unchanged wrapper macros use GNU named variadic arguments.
target_compile_options(io_uring_recorder_test PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-variadic-macros)
enable_project_tools(${PROJECT_NAME})

install(TARGETS io_uring_recorder_test
    RUNTIME DESTINATION ${INSTALL_DIR}/bin/io_uring_recorder_test
    OPTIONAL
)
