#pragma once
#include <string>
#include <vector>

class CustomClass42 {
public:
    int add(int x , int y);
    std::string to_string();
    void sort(std::vector<int>& numbers);
private:
    int _x{0};
};
