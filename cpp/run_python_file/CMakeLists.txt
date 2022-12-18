cmake_minimum_required(VERSION 3.22)
project(run_python_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    main.cpp
)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})


add_executable(run_python_test ${SOURCE_FILES})

target_link_libraries(run_python_test
    ${PYTHON_LIBRARIES}
    stdc++fs
)
