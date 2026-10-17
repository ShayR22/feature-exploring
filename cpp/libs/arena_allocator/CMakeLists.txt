project(arena_allocator_lib)

add_library(${PROJECT_NAME} STATIC
    src/arena_allocator.cpp
)
add_library(feature_exploring::arena_allocator ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
install_project_library(${PROJECT_NAME})
enable_project_tools(${PROJECT_NAME})
