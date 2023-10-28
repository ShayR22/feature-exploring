#include "custom_class_2.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass2::add(int x, int y) {
    return x + y;
}

std::string CustomClass2::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass2::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}