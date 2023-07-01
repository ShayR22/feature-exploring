#include "shape_te.hpp"
#include "shapes/circle.hpp"
#include "shapes/rect.hpp"
#include "shapes_drawer/shape_drawing.hpp"

#include <vector>
#include <iostream>

void span_buffer_printer(std::span<const uint8_t> buffer) {
    std::cout << "Bytes: ";
    for (const auto& byte: buffer) {
        std::cout << static_cast<uint32_t>(byte) << ", ";
    }
    std::cout << "\n";
}

int main() {

    using namespace up_shape_te;
    std::vector<Shape> shapes;

    shapes.push_back({Circle{1}, draw_circle});
    shapes.push_back({Circle{2}, draw_circle});
    shapes.push_back({Rect{4, 2}, draw_rect, default_stack_serializtion<Rect>});
    shapes.push_back({Circle{120}, draw_circle, default_stack_serializtion<Circle>});

    for (const auto& shape: shapes) {
        shape.draw();
        std::span<const uint8_t> serialized_data = shape.serialize();
        span_buffer_printer(serialized_data);
    }

    return 0;
}
