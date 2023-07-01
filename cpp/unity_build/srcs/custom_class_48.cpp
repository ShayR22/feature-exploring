#include "custom_class_48.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass48::add(int x, int y) {
    return x + y;
}

std::string CustomClass48::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass48::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}