
cmake_minimum_required(VERSION 3.22)
project(kita_alef)

SET(HEADERS
    include/kita_alef/add.hpp
    include/kita_alef/sub.hpp
)

SET(SRCS
    srcs/add.cpp
    srcs/sub.cpp
)

add_library(kita_alef STATIC ${SRCS})
target_include_directories(kita_alef PUBLIC include)

install(TARGETS kita_alef DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/kita_alef)

