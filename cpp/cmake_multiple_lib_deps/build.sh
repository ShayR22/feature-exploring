#!/bin/bash

rm -rf build
mkdir build

pushd ./build
echo "Generating CMake"
cmake ../
echo "Building Project"
cmake --build .
echo "Intalling Project"
cmake --install .
popd
