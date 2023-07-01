#!/bin/bash


NUM_CLASSES=100
for i in $(seq 0 $NUM_CLASSES); do
	# generate header
	CLASS_NAME="CustomClass$i"
	# generate header file
	printf \
"#pragma once
#include <string>
#include <vector>

class %s {
public:
    int add(int x , int y);
    std::string to_string();
    void sort(std::vector<int>& numbers);
private:
    int _x{0};
};
" $CLASS_NAME > custom_class_$i.hpp

	# generate src file
	printf \
"#include \"custom_class_%d.hpp\"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int %s::add(int x, int y) {
    return x + y;
}

std::string %s::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void %s::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}" $i $CLASS_NAME $CLASS_NAME $CLASS_NAME > custom_class_$i.cpp

done

UNITY_BUILD_FILE="../unity_build.cpp";
if [ -f ${UNITY_BUILD_FILE} ]; then
    rm "${UNITY_BUILD_FILE}"
fi

touch "${UNITY_BUILD_FILE}"

for i in $(seq 0 $NUM_CLASSES); do
	printf \
"#include \"srcs/custom_class_%d.cpp\"\n" $i >> "${UNITY_BUILD_FILE}"
done
