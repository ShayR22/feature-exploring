#include "custom_class_10.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass10::add(int x, int y) {
    return x + y;
}

std::string CustomClass10::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass10::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}