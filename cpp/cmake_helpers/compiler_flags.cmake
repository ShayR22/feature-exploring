
# set c/cpp compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -fPIC")
set(CMAKE_CXX_FLAGS "-std=c++20 ${CMAKE_C_FLAGS}")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS_RELEASE "-O2")

if (${USE_CLANG_BUILDER_ANALYZER})
    # in order to run clang build analyzer in the background -ftime-trace must be on
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftime-trace")
endif()

# set linker to be clang llvm default linker the lld linker
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")


