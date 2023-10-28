#include "logger.h"
#include <cstdint>
#include "dir_x/dir_y/print_x.h"

int main(int argc, const char* argv[]) {
    uint32_t x = 42;
    INFO("ggwp x=%u", x);
    FO_INFO("ggwp x=%u", x);
    print_x(x);

    return 0;
}