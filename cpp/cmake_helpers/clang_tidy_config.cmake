find_program(CLANG_TIDY_PATH "clang-tidy")
if (CLANG_TIDY_PATH)
    message(STATUS "found CLANG_TIDY_PATH=${CLANG_TIDY_PATH}")
else()
    message(FATAL_ERROR "Failed to find clang-tidy")
endif()


# add all modernize checks "modernize*" and remove specific checks
set(CLANG_TIDY_CHECKS_MODERNIZE
    "modernize*,-modernize-use-trailing-return-type*,-modernize-use-auto*,-modernize-avoid-c-arrays*"
)

# add all modernize checks "cppcoreguidelines*" and remove specific checks
set(CLANG_TIDY_CHECKS_CPP_CORE_GUIDE
    "cppcoreguidelines*,-cppcoreguidelines-pro-bounds-array-to-pointer-decay*, \
    -cppcoreguidelines-pro-type-union-access*, -cppcoreguidelines-avoid-non-const-global-variables*, \
    -cppcoreguidelines-pro-type-reinterpret-cast*,-cppcoreguidelines-pro-type-vararg*, \
    -cppcoreguidelines-avoid-c-arrays"
)


# add all performance checks, mainly copy-ctor hidden allocation
set(CLANG_TIDY_CHECKS_PERFORMANCE
    "performance*"
)

# checks code poratbility
set(CLANG_TIDY_CHECKS_PORTABILITY
    "portability*"
)

# remove all checks and add each type of cheks
set(CLANG_TIDY_CHECKS
  "-*,${CLANG_TIDY_CHECKS_MODERNIZE},${CLANG_TIDY_CHECKS_CPP_CORE_GUIDE}, \
   ${CLANG_TIDY_CHECKS_PERFORMANCE},${CLANG_TIDY_CHECKS_PORTABILITY}"
)

# filter regex for clang tidy in order to filter third parties or code that we
# don't want to be checked this inlcude:
# - 3rd-parties such as nlohmann
set(CLANG_TIDY_HEADER_FILTER "nlohmann")
set(CLANG_TIDY_HEADER_FILTER_REGEX "^(?!.*(?:${CLANG_TIDY_HEADER_FILTER})).*\.(h|hpp)$")

set(CLANG_TIDY_PATH_WITH_OPTIONS
    "${CLANG_TIDY_PATH}" "-format-style=file" "-checks=${CLANG_TIDY_CHECKS}" "-header-filter=${CLANG_TIDY_HEADER_FILTER_REGEX}"
)
