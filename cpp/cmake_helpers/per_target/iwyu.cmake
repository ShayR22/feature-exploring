# Per-target include-what-you-use opt-in.
# Call enable_iwyu(<target>) after defining your target.
include_guard(GLOBAL)

function(enable_iwyu target)
    if(NOT TARGET "${target}")
        message(FATAL_ERROR "enable_iwyu: target '${target}' does not exist")
    endif()
    if(NOT ENABLE_IWYU)
        return()
    endif()
    message(VERBOSE "Enabling include-what-you-use for ${target}")
    set_property(TARGET "${target}" PROPERTY CXX_INCLUDE_WHAT_YOU_USE "${IWYU_OPTIONS}")
endfunction()
