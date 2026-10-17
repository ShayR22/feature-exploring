include_guard(GLOBAL)

function(install_project_library target)
    if(NOT TARGET "${target}")
        message(FATAL_ERROR "install_project_library: target '${target}' does not exist")
    endif()
    message(VERBOSE "setting ${target} install to ${INSTALL_DIR}/lib/${target}")
    install(TARGETS "${target}"
        ARCHIVE DESTINATION "${INSTALL_DIR}/lib/${target}"
        OPTIONAL
        LIBRARY DESTINATION "${INSTALL_DIR}/lib/${target}"
        OPTIONAL
    )

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        OPTIONAL
        DESTINATION "${INSTALL_DIR}/include"
        FILES_MATCHING PATTERN "*.h*"
    )
endfunction()
