#include "custom_class_3.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass3::add(int x, int y) {
    return x + y;
}

std::string CustomClass3::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass3::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}