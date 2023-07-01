#include "custom_class_41.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass41::add(int x, int y) {
    return x + y;
}

std::string CustomClass41::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass41::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}