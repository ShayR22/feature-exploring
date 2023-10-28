#include "custom_class_65.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass65::add(int x, int y) {
    return x + y;
}

std::string CustomClass65::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass65::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}