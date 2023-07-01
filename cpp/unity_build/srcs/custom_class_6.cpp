#include "custom_class_6.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass6::add(int x, int y) {
    return x + y;
}

std::string CustomClass6::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass6::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}