# Per-target clang-tidy opt-in.
# Call enable_clang_tidy(<target>) after defining your target.
include_guard(GLOBAL)

function(enable_clang_tidy target)
    if(NOT TARGET "${target}")
        message(FATAL_ERROR "enable_clang_tidy: target '${target}' does not exist")
    endif()
    if(NOT ENABLE_CLANG_TIDY)
        return()
    endif()
    message(VERBOSE "Enabling clang-tidy for ${target}")
    set_property(TARGET "${target}" PROPERTY CXX_CLANG_TIDY "${CLANG_TIDY_OPTIONS}")
endfunction()
