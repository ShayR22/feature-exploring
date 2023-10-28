project(func_measuring)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

include(${CMAKE_INSTALL_LIB_HELPER})
include(${CMAKE_TOOLS_PATH})