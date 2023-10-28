#include "custom_class_61.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass61::add(int x, int y) {
    return x + y;
}

std::string CustomClass61::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass61::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}