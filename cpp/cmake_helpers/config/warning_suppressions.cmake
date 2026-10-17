# Consolidated warning suppressions
#
# This is the single place where every per-target exception to the global
# warning policy (cmake_helpers/config/compiler_flags.cmake) lives.
#
# Included at the end of the root CMakeLists.txt, after every
# add_subdirectory() call has defined its targets.
#
# Example:
#   suppress_warnings(my_legacy_lib
#       -Wno-unused-parameter
#       -Wno-sign-compare
#   )

function(suppress_warnings target)
    if (NOT TARGET ${target})
        return()
    endif()
    target_compile_options(${target} PRIVATE ${ARGN})
endfunction()

# Existing demonstrations intentionally ignore command-line arguments.
suppress_warnings(arena_allocator -Wno-unused-parameter)
suppress_warnings(base_file_name -Wno-unused-parameter)
suppress_warnings(type_erasure_dispatcher -Wno-unused-parameter)
# Retain the imported wrapper's local counter and the existing experiment code.
suppress_warnings(gtest_wrapper -Wno-shadow)
suppress_warnings(buf_to_string -Wno-ignored-qualifiers)
suppress_warnings(atmomic_vs_lock -Wno-missing-noreturn)
# func_measuring and these consumers intentionally use legacy numeric casts.
foreach(target buf_to_string malloc_tests set_vs_array_tests simd_memcpy
        standard_for_each std_map_tests std_map_tests_map_generator type_erasure
        list_vs_vector_isnert_erase inplace_transpose)
    suppress_warnings(${target} -Wno-old-style-cast)
endforeach()
