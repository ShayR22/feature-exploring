#include "../../func_measuring/tester.hpp"
#include "../../type_erasure/shapes/circle.hpp"
#include "../../type_erasure/shapes/rect.hpp"
#include "../../type_erasure/shapes_drawer/shape_drawing.hpp"
#include "../../type_erasure/unique_ptr_type_erasure/shape_te.hpp"
#include "../../type_erasure/small_buffer_optimization_type_erasure/shape_te.hpp"

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

static void serialization_test(const std::vector<auto>& serializables, uint32_t num_serializations) {
    for (uint32_t i = 0; i < num_serializations; i++) {
        for (const auto& serialiable: serializables) {
            std::span<const uint8_t> data = serialiable.serialize();
            if (data.size() > 100) {
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

        sbo_shapes.push_back(sbo_shape_te::Shape<>(circle, draw_circle));
        sbo_shapes.push_back(sbo_shape_te::Shape<>(rect, draw_rect));


        up_shapes.push_back(up_shape_te::Shape(circle, draw_circle));
        up_shapes.push_back(up_shape_te::Shape(rect, draw_rect));
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

static void print_going_to(uint32_t allocation_size) {
    float allocation_size_f = allocation_size;
    std::string allocation_size_suffix = "Bytes";

    if (allocation_size_f >= 1<<30) {
        allocation_size_f /= 1<<30;
        allocation_size_suffix = "GB";
    }

    if (allocation_size_f >= 1<<20) {
        allocation_size_f /= 1<<20;
        allocation_size_suffix = "MB";
    }

    if (allocation_size_f >= 1<<10) {
        allocation_size_f /= 1<<10;
        allocation_size_suffix = "KB";
    }

    std::string allocation_size_str = _to_string_with_precision(allocation_size_f);
    printf("\nGoing to test allocation with %s[%s] shapes\n", allocation_size_str.c_str(), allocation_size_suffix.c_str());
}

int main() {
    for(uint32_t i = 4; i <= 18; i++) {
        uint32_t allocation_size = 1 << i;
        print_going_to(allocation_size);
        run_shapes_tests(allocation_size, 1);
    }
}

