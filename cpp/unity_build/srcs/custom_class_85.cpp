#include "custom_class_85.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass85::add(int x, int y) {
    return x + y;
}

std::string CustomClass85::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass85::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}