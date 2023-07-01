#include "shape_te.hpp"
#include "shapes/circle.hpp"
#include "shapes/rect.hpp"
#include "shapes_drawer/shape_drawing.hpp"
#include <iostream>
#include <vector>

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
