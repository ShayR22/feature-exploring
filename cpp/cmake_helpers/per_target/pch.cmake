# Opt in after defining a target; analysis runs on the original source files.
if(NOT ENABLE_IWYU AND NOT ENABLE_CLANG_TIDY AND NOT DISABLE_PCH)
    target_precompile_headers(${PROJECT_NAME} PRIVATE <string> <vector> <memory>)
endif()
