#include "custom_class_88.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass88::add(int x, int y) {
    return x + y;
}

std::string CustomClass88::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass88::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}