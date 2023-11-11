project(type_erasure)

add_executable(${PROJECT_NAME}
    srcs/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../features/type_erasure")

target_link_libraries(${PROJECT_NAME} PRIVATE func_measuring)

include(${CMAKE_INSTALL_BIN_HELPER})
include(${CMAKE_TOOLS_PATH})