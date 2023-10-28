find_program(IWYU_PATH NAMES include-what-you-use iwyu REQUIRED)
if (IWYU_PATH)
    message(STATUS "found IWYU_PATH=${IWYU_PATH}")
else()
    message(FATAL_ERROR "Failed to find iwyu")
endif()

# IWYU_PATH_AND_OPTIONS is a setter for running the tool with flags.
# -Xiwyu is a type if argument that require flag to be passed for better understanding
# view include-what-you-use --help
# no-comments flag remove addition comments from headers, the comments are what values
# from the header, for best understanding remove flag and run the tool
# --verbose level differ in values and no much comments about it, seems 1,5,7 are good numbers to
# work with (this should be just changed in CMake when developing)
set(IWYU_PATH_AND_OPTIONS
    ${IWYU_PATH}
    -Xiwyu --verbose=1
    -Xiwyu --no_comments
)
