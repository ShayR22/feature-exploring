#include "func_measuring/tester.hpp"
#include "shapes/circle.hpp"
#include "shapes/rect.hpp"
#include "shapes_drawer/shape_drawing.hpp"
#include "unique_ptr_type_erasure/shape_te.hpp"
#include "small_buffer_optimization_type_erasure/shape_te.hpp"

#include <vector>
#include <memory>
#include <cstring>


static std::vector<Rect> generatre_rectangles(uint32_t num_rects) {
    std::vector<Rect> rects(num_rects);
    for (uint32_t i = 0; i < num_rects; i++) {
        int w = rand();
        int h = rand();
        rects[i] = Rect(w, h);
    }
    return rects;
}

static std::vector<Circle> generatre_circles(uint32_t num_circles) {
    std::vector<Circle> circles(num_circles);
    for (uint32_t i = 0; i < num_circles; i++) {
        int r = rand();
        circles[i] = Circle(r);
    }
    return circles;
}


template<typename T>
static void serialization_test(const std::vector<T>& serializables, uint32_t num_serializations) {
    static constexpr auto PRINT_SIZE = 100; // random number

    for (uint32_t i = 0; i < num_serializations; i++) {
        for (const auto& serialiable: serializables) {
            std::span<const uint8_t> data = serialiable.serialize();
            if (data.size() > PRINT_SIZE) {
                printf("gg data.size() > 100\n");
            }
        }
    }
}

static void run_shapes_tests(uint32_t num_shapes, uint32_t num_serializations=1) {
    if (num_shapes % 2 == 1) {
        printf("fml why not module 2\n");
        num_shapes++;
    }
    uint32_t num_other_shapes = num_shapes / 2;
    std::vector<Circle> circles = generatre_circles(num_other_shapes);
    std::vector<Rect> rects = generatre_rectangles(num_other_shapes);

    std::vector<sbo_shape_te::Shape<>> sbo_shapes;
    std::vector<up_shape_te::Shape> up_shapes;

    for (uint32_t i = 0; i < num_shapes; i += 2) {
        const Circle& circle = circles[i/2];
        const Rect& rect = rects[i/2];

        sbo_shapes.emplace_back(circle, draw_circle);
        sbo_shapes.emplace_back(rect, draw_rect);


        up_shapes.emplace_back(circle, draw_circle);
        up_shapes.emplace_back(rect, draw_rect);
    }

    auto serialize_sbo = [&]() {
        serialization_test(sbo_shapes, num_serializations);
    };

    auto serialize_up = [&]() {
        serialization_test(up_shapes, num_serializations);
    };

    RUN_TEST(serialize_up);
    RUN_TEST(serialize_sbo);

}

int main() {
    static constexpr uint8_t TWO_POWER_MIN_ALLOC = 4;
    static constexpr uint8_t TWO_POWER_MAX_ALLOC = 18;
    for (uint32_t i = TWO_POWER_MIN_ALLOC; i <= TWO_POWER_MAX_ALLOC; i++) {
        uint32_t allocation_size = 1 << i;
        print_allocation_size(allocation_size);
        run_shapes_tests(allocation_size, 1);
    }
}

