project(atmomic_vs_lock)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
)

include(${CMAKE_INSTALL_BIN_HELPER})
include(${CMAKE_TOOLS_PATH})