#include "custom_class_81.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass81::add(int x, int y) {
    return x + y;
}

std::string CustomClass81::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass81::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}