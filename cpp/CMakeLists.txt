cmake_minimum_required(VERSION 3.22)
project("feature-exploring")

# User parameters
set(CMAKE_BUILD_TYPE
    ""
    CACHE
    STRING
    "Build type - Release/Debug"
)

set(BUILD_INSTALL_PATH
    ""
    CACHE
    STRING
    "Build install path, path to where under build directory where targets would be installed"
)

# User parameters validations
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
if (NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "no build type given. please add -DCMAKE_BUILD_TYPE=Release|Debug")
endif()

message(STATUS "BUILD_INSTALL_PATH=${BUILD_INSTALL_PATH}")
if (NOT BUILD_INSTALL_PATH)
    message(FATAL_ERROR "no build install path given. please add -DBUILD_INSTALL_PATH=install")
endif()

########################################

# set tools and helper cmake used by project
set(CMAKE_HELPERS_PATH "${CMAKE_SOURCE_DIR}/cmake_helpers")
set(CMAKE_TOOLS_CONFIG_PATH "${CMAKE_HELPERS_PATH}/tool_helper_config.cmake")
set(CMAKE_TOOLS_PATH "${CMAKE_HELPERS_PATH}/tool_helper.cmake")

message(VERBOSE "CMAKE_TOOLS_CONFIG_PATH=${CMAKE_TOOLS_CONFIG_PATH}")
message(VERBOSE "CMAKE_TOOLS_PATH=${CMAKE_TOOLS_PATH}")


# configure tools
if (${USE_IWYU} OR ${USE_CLANG_TIDY})
    message(STATUS "Configuring CMAKE_TOOLS_CONFIG_PATH=${CMAKE_TOOLS_CONFIG_PATH}")
    include(${CMAKE_TOOLS_CONFIG_PATH})
endif()

set(CMAKE_INSTALL_BIN_HELPER "${CMAKE_HELPERS_PATH}/install_binary.cmake")
set(CMAKE_INSTALL_LIB_HELPER "${CMAKE_HELPERS_PATH}/install_library.cmake")

message(VERBOSE "CMAKE_INSTALL_BIN_HELPER=${CMAKE_INSTALL_BIN_HELPER}")
message(VERBOSE "CMAKE_INSTALL_LIB_HELPER=${CMAKE_INSTALL_LIB_HELPER}")

# Set the install destination to the build directory
set(INSTALL_DIR "${BUILD_INSTALL_PATH}")
message(VERBOSE "INSTALL_DIR=${INSTALL_DIR}")

add_subdirectory("features")
add_subdirectory("tests")