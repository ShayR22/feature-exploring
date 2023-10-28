cmake_minimum_required(VERSION 3.22)
project("libs_deps")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# CTCE -> Clang Tidy Checks Exclude
set(CTCE_CPP "-cppcoreguidelines-pro-type-vararg*,-hicpp-vararg*")
set(CTCE_MODERN "-modernize-use-trailing-return-type*")
set(CTCE_LLVM "-llvm*namespace*,-llvmlibc-restrict-system-libc-header*,-llvm-header-guard*")
set(CTCE_ALTERA "-altera-unroll-loops*")
set(CTCE "${CTCE_CPP},${CTCE_MODERN},${CTCE_LLVM},${CTCE_ALTERA}")


set(CMAKE_CXX_CLANG_TIDY
  clang-tidy;
  -format-style='file';
  -header-filter=${CMAKE_CURRENT_SOURCE_DIR};
  -checks=*,${CTCE}
  -warnings-as-errors=*;
)

# Set the install destination to the build directory
set(INSTALL_DIR "${CMAKE_BINARY_DIR}/install")

add_subdirectory("kita_alef")
add_subdirectory("kita_bet")
add_subdirectory("my_main")