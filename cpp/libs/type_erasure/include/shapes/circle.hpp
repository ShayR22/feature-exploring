#pragma once

class Circle {
public:
    Circle() : _radius{0} {}
    Circle(int radius) : _radius{radius} {}
    Circle(const Circle& lhs) = default;
    Circle(Circle&& rhs) = default;
    Circle& operator=(const Circle& lhs) = default;
    Circle& operator=(Circle&& rhs) = default;

    int get_radius() const { return _radius; }

private:
    int _radius;
};
