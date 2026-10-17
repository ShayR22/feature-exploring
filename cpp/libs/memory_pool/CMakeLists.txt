project(memory_pool_lib)

add_library(${PROJECT_NAME} INTERFACE
)
add_library(feature_exploring::memory_pool ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
install_project_library(${PROJECT_NAME})
