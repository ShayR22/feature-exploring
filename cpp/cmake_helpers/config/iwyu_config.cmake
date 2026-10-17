# include-what-you-use configuration
if(ENABLE_IWYU)
    find_program(IWYU_EXE NAMES include-what-you-use iwyu)
    if(IWYU_EXE)
        message(NOTICE "IWYU found: ${IWYU_EXE}")

        set(IWYU_OPTIONS
            "${IWYU_EXE}"
            "-Xiwyu" "--verbose=1"
            "-Wno-unknown-warning-option"
            "-Wno-error"
        )
    else()
        message(WARNING "ENABLE_IWYU is ON but include-what-you-use not found")
        set(ENABLE_IWYU OFF)
    endif()
endif()
