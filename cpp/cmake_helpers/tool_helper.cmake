include_guard(GLOBAL)
include("${CMAKE_CURRENT_LIST_DIR}/per_target/clang_tidy.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/per_target/iwyu.cmake")

# Apply the configured analysis tools to an explicitly registered target.
function(enable_project_tools target)
    enable_clang_tidy("${target}")
    enable_iwyu("${target}")
endfunction()
