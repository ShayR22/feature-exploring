#include "custom_class_93.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass93::add(int x, int y) {
    return x + y;
}

std::string CustomClass93::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass93::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}