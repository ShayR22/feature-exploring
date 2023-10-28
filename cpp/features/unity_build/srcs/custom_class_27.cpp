#include "custom_class_27.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass27::add(int x, int y) {
    return x + y;
}

std::string CustomClass27::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass27::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}