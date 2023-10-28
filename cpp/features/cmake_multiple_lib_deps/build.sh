#!/bin/bash

IS_DIR_CLEAN=0
IS_CLEAN=0
IS_BUILD=0
IS_INSTALL=0
IS_GENERATE_GRAPH=0

function usage() {
    echo "Usage: $0 [-b|--build] [-c|--clean] [-d|--directory-clean] [-i|--install] [-g|--graph-dependency]"
}

# Process arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d | --directory-clean)
            IS_DIR_CLEAN=1
            ;;
        -c | --clean)
            IS_CLEAN=1
            ;;
        -b | --build)
            IS_BUILD=1
            ;;
        -i | --install)
            IS_INSTALL=1
            ;;
        -g | --graph-dependency)
            IS_GENERATE_GRAPH=1
            ;;
        -h | --help)
            usage
            exit 0
            ;;
        *)
            echo "Invalid argument: $1"
            usage
            exit 1
            ;;
    esac
    shift
done


if [ -d "build" ]; then
    echo "Found build directory under build"
    if [ "$IS_DIR_CLEAN" -ne 0 ]; then
        echo "clean flag is on going to delete dir and recreate it"
        rm -r "build"
        mkdir "build"
    fi

else
    echo "Build directory does not exist. creating build directory"
    mkdir "build"
fi

pushd ./build

export CC="/usr/bin/clang"
export CXX="/usr/bin/clang++"

cmake_cmd="cmake ../"
if [ $IS_GENERATE_GRAPH -ne 0 ]; then
    cmake_cmd="$cmake_cmd -DGRAPHVIZ_EXTERNAL_LIBS=OFF --graphviz=./graph/graph"
fi

echo "Generating CMake with cmd=$cmake_cmd"
$cmake_cmd

if [ $IS_BUILD -ne 0 ]; then
    echo "Building Project"
    build_cmd="cmake --build ."
    if [ $IS_CLEAN -ne 0 ]; then
        build_cmd="$build_cmd --clean-first"
    fi

    $build_cmd
fi

if [ $IS_INSTALL -ne 0 ]; then
    echo "Intalling Project"
    cmake --install .
fi

popd
