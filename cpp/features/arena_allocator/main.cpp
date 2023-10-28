#include "arena_allocator.hpp"
#include <vector>

static constexpr uint32_t KB = 1 << 10;
static constexpr uint32_t MAIN_ALLOCATION_SIZE_BYTES = KB;


Allocation galloc(uint32_t size) {
    static ArenaAllocator arena{MAIN_ALLOCATION_SIZE_BYTES};
    return arena.allocate(size);
}

void foo() {
    Allocation f = galloc(KB/2);
}

void goo() {
    Allocation g1 = galloc(KB/2);
    Allocation g2 = galloc(KB/2);
}

void bar() {
    Allocation b1 = galloc(KB/2);
    Allocation b2 = galloc(KB/2);
    Allocation b3 = galloc(KB/2);
}

int main(int argc, const char* argv[]) {
    foo();
    goo();
    bar();
    return 0;
}