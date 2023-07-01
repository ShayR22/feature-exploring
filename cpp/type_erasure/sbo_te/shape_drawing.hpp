#pragma once

#include "circle.hpp"
#include "rect.hpp"
#include <cstdio>

void draw_circle(const Circle& circle) {
    printf("drawing circle with radius=[%d]\n", circle.get_radius());
}

void draw_rect(const Rect& rect) {
    printf("drawing rect with w=[%d] h=[%d]\n", rect.get_w(), rect.get_h());
}