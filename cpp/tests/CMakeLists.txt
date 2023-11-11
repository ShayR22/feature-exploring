cmake_minimum_required(VERSION 3.22)
project("tests")

add_subdirectory("buf_to_string")
add_subdirectory("atomic_vs_lock")
add_subdirectory("malloc_tests")
add_subdirectory("set_vs_array_tests")
add_subdirectory("simd_memcpy")
add_subdirectory("spdlog_test")