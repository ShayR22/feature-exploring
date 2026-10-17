project(io_uring_recorder)
find_package(Threads REQUIRED)
find_library(URING_LIBRARY NAMES uring)
find_path(URING_INCLUDE_DIR NAMES liburing.h)
if(NOT URING_LIBRARY OR NOT URING_INCLUDE_DIR)
    message(FATAL_ERROR "io_uring_recorder requires liburing development headers and library (liburing-dev)")
endif()

add_library(io_uring_recorder STATIC
    srcs/executor/io_uring_executor.cpp
    srcs/executor/request_pool.cpp
    srcs/io/io_uring.cpp
    srcs/recorder/io_uring_recorder.cpp
    srcs/rotation/rotating_recorder.cpp
)

add_library(io_uring_recorder::io_uring_recorder ALIAS io_uring_recorder)

target_compile_features(io_uring_recorder PUBLIC cxx_std_20)

target_include_directories(io_uring_recorder
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/srcs
        ${URING_INCLUDE_DIR}
)

target_link_libraries(io_uring_recorder PRIVATE ${URING_LIBRARY} Threads::Threads)

target_compile_options(io_uring_recorder PRIVATE -Wall -Wextra -Wpedantic -Werror)

enable_project_tools(${PROJECT_NAME})

install(TARGETS io_uring_recorder
    ARCHIVE DESTINATION ${INSTALL_DIR}/lib/io_uring_recorder OPTIONAL
    LIBRARY DESTINATION ${INSTALL_DIR}/lib/io_uring_recorder OPTIONAL
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    OPTIONAL DESTINATION ${INSTALL_DIR}/include
    FILES_MATCHING PATTERN "*.h*"
)
