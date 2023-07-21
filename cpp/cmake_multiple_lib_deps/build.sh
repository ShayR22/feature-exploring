#!/bin/bash

rm -rf build
mkdir build

pushd ./build

export CC="/usr/bin/clang"
export CXX="/usr/bin/clang++"

echo "Generating CMake"
cmake ../
echo "Building Project"
cmake --build .
echo "Intalling Project"
cmake --install .
popd
