cmake_minimum_required(VERSION 3.22)
project("tests")

add_subdirectory("buf_to_string")
add_subdirectory("atomic_vs_lock")
add_subdirectory("malloc_tests")
add_subdirectory("set_vs_array_tests")
add_subdirectory("simd_memcpy")
add_subdirectory("spdlog_test")
add_subdirectory("standard_for_each")
add_subdirectory("std_map_tests")
add_subdirectory("type_erasure")
add_subdirectory("list_vs_vector_insert_erase")
add_subdirectory("inplace_transpose")

