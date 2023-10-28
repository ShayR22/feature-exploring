#include "custom_class_71.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass71::add(int x, int y) {
    return x + y;
}

std::string CustomClass71::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass71::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}