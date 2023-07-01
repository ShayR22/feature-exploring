#include "custom_class_95.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass95::add(int x, int y) {
    return x + y;
}

std::string CustomClass95::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass95::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}