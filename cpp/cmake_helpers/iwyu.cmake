if (${USE_IWYU})
    message(STATUS "setting ${PROJECT_NAME} to use include-what-you-use")
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH_AND_OPTIONS})
endif()
