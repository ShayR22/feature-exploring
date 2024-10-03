project(c_hashmap)
cmake_minimum_required(VERSION 3.22)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
add_library(${PROJECT_NAME} srcs/c_hashmap.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)

add_executable(${PROJECT_NAME}_test main.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})

# include(${CMAKE_INSTALL_BIN_HELPER})
# include(${CMAKE_TOOLS_PATH})