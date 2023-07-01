#pragma once

class Circle {
public:
    Circle(int radius) : _radius{radius} {}
    int get_radius() const { return _radius; }

private:
    int _radius;
};
