if (${USE_CLANG_TIDY})
    message(VERBOSE "setting clang tidy to ${PROJECT_NAME}")
    set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_PATH_WITH_OPTIONS}")
endif()
