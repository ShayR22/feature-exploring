# install_dir/bin/target_name/executable
include_guard(GLOBAL)

function(install_project_binary target)
    if(NOT TARGET "${target}")
        message(FATAL_ERROR "install_project_binary: target '${target}' does not exist")
    endif()
    message(VERBOSE "setting ${target} install to ${INSTALL_DIR}/bin/${target}")
    install(TARGETS "${target}"
        RUNTIME DESTINATION "${INSTALL_DIR}/bin/${target}"
        OPTIONAL
    )
endfunction()
