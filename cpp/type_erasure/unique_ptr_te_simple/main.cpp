#include "circle.hpp"
#include "rect.hpp"
#include "shape_drawing.hpp"
#include "shape_te.hpp"

#include <vector>
#include <iostream>

int main() {
    std::vector<Shape> shapes;

    shapes.push_back({Circle{1}, draw_circle});
    shapes.push_back({Circle{2}, draw_circle});
    shapes.push_back({Rect{4, 2}, draw_rect});
    shapes.push_back({Circle{120}, draw_circle});

    for (const auto& shape: shapes) {
        shape.draw();
    }
}
