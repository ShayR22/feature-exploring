project(custom_optional)

add_library(${PROJECT_NAME} INTERFACE)
add_library(feature_exploring::custom_optional ALIAS ${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
install_project_library(${PROJECT_NAME})

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
