cmake_minimum_required(VERSION 3.22)
project("feature-exploring" LANGUAGES CXX)

# Build options apply to the container build and direct CMake invocations.
option(BUILD_EXPERIMENTS "Build standalone experiments" ON)
option(BUILD_TESTS "Build library tests" ON)
option(USE_UNITY_BUILD "Enable opted-in Unity builds" OFF)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UBSan" OFF)
option(ENABLE_THREAD_SANITIZER "Enable ThreadSanitizer in a separate build" OFF)
set(BUILD_LINKER "lld" CACHE STRING "Linker: lld or mold")
set_property(CACHE BUILD_LINKER PROPERTY STRINGS lld mold)
option(ENABLE_CLANG_TIDY "Enable clang-tidy checks during compilation" OFF)
option(ENABLE_IWYU "Enable include-what-you-use analysis" OFF)
option(ENABLE_CCACHE "Enable ccache for faster rebuilds" OFF)
option(DISABLE_PCH "Disable precompiled headers" ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    string(TOLOWER "${CMAKE_BUILD_TYPE}" build_type_directory)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/build/${build_type_directory}/install"
        CACHE PATH "Installation directory" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
# Global configuration and per-target opt-in helpers.
set(CMAKE_HELPERS_PATH "${CMAKE_SOURCE_DIR}/cmake_helpers")
set(CMAKE_CONFIG_PATH "${CMAKE_HELPERS_PATH}/config")
set(CMAKE_TARGET_PATH "${CMAKE_HELPERS_PATH}/per_target")
include("${CMAKE_CONFIG_PATH}/compiler_flags.cmake")
include("${CMAKE_CONFIG_PATH}/linker_config.cmake")
include("${CMAKE_CONFIG_PATH}/sanitizers.cmake")
include("${CMAKE_CONFIG_PATH}/ccache_config.cmake")
include("${CMAKE_CONFIG_PATH}/clang_tidy_config.cmake")
include("${CMAKE_CONFIG_PATH}/iwyu_config.cmake")
set(CMAKE_PCH_TARGET "${CMAKE_TARGET_PATH}/pch.cmake")
include("${CMAKE_TARGET_PATH}/unity.cmake")

# Load tool and installation function definitions once for all subdirectories.
include("${CMAKE_HELPERS_PATH}/tool_helper.cmake")
include("${CMAKE_HELPERS_PATH}/install_binary.cmake")
include("${CMAKE_HELPERS_PATH}/install_library.cmake")

# Relative install destinations follow CMAKE_INSTALL_PREFIX.
set(INSTALL_DIR ".")

add_subdirectory("features")
if(BUILD_TESTS)
    add_subdirectory("test_support")
endif()
# Existing standalone experiments still reside here until the layout migration.
if(BUILD_TESTS OR BUILD_EXPERIMENTS)
    add_subdirectory("tests")
endif()

# Apply exceptions after all targets have been defined.
include("${CMAKE_CONFIG_PATH}/warning_suppressions.cmake")
