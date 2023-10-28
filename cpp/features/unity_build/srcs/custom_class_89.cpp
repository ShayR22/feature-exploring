#include "custom_class_89.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass89::add(int x, int y) {
    return x + y;
}

std::string CustomClass89::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass89::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}