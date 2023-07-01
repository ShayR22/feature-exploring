#include "custom_class_52.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass52::add(int x, int y) {
    return x + y;
}

std::string CustomClass52::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass52::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}