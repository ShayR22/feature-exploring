#include "custom_class_76.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass76::add(int x, int y) {
    return x + y;
}

std::string CustomClass76::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass76::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}