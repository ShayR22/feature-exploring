# install_dir/bin/project_name/executable

message(VERBOSE "setting ${PROJECT_NAME} install to ${INSTALL_DIR}/bin/${PROJECT_NAME}")
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${INSTALL_DIR}/bin/${PROJECT_NAME}
    OPTIONAL
)