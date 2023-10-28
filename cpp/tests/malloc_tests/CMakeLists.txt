project(malloc_tests)

add_executable(${PROJECT_NAME}
    srcs/allocate.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE func_measuring)

include(${CMAKE_INSTALL_BIN_HELPER})
include(${CMAKE_TOOLS_PATH})