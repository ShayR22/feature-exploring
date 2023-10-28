#include "custom_class_68.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass68::add(int x, int y) {
    return x + y;
}

std::string CustomClass68::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass68::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}