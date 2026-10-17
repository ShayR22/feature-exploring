# Keep the compiler and language standard consistent with the Conan profiles.
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "This project requires Clang; use the Conan profiles or set CXX=clang++-18")
endif()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Clang warnings; existing code exceptions are kept in warning_suppressions.cmake.
add_compile_options(
    -Wall -Wextra -Werror
    -Wshift-overflow -Wshadow -Wformat=2 -Wpointer-arith
    -Wmissing-declarations -Wnull-dereference -Wnon-virtual-dtor
    -Woverloaded-virtual -Wsuggest-override -Wformat-security
    -Wredundant-decls -Wunused-macros -Wvla
    -Wzero-as-null-pointer-constant -Wmissing-noreturn
    -Wcast-qual -Wcast-align -Wold-style-cast -Wfloat-equal
    $<$<CONFIG:Debug,RelWithDebInfo>:-fno-omit-frame-pointer>
)
