#!/bin/bash


IS_CLEAN=0
IS_BUILD=0
IS_INSTALL=0

# Process arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -c | --clean)
            IS_CLEAN=1
            ;;
        -b | --build)
            IS_BUILD=1
            ;;
        -i | --install)
            IS_INSTALL=1
            ;;
        *)
            echo "Invalid argument: $1"
            echo "Usage: $0 [-c|--clean]"
            exit 1
            ;;
    esac
    shift
done


if [ -d "build" ]; then
    echo "Found build directory under build"
    if [ "$IS_CLEAN" -ne 0 ]; then
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

echo "Generating CMake"
cmake ../

if [ "$IS_BUILD" -ne 0 ]; then
    echo "Building Project"
    cmake --build .
fi

if [ "$IS_INSTALL" -ne 0 ]; then
    echo "Intalling Project"
    cmake --install .
fi

popd
