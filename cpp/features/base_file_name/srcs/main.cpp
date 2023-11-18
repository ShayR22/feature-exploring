#include "logger.h"
#include <cstdint>
#include "dir_x/dir_y/print_x.h"

int main(int argc, const char* argv[]) {
    static constexpr auto MEANING_OF_LIFE = 42;
    uint32_t x = MEANING_OF_LIFE;
    INFO("ggwp x=%u", x);
    FO_INFO("ggwp x=%u", x);
    print_x(x);

    return 0;
}