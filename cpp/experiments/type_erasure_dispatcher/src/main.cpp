#include "fsm.hpp"

int main(int argc, const char* argv[]) {
    Fsm fsm{};

    fsm.x({});
    fsm.y({});
    fsm.z({.z = 1});
    fsm.x({});
    fsm.z({.z = -1});

    return 0;
}