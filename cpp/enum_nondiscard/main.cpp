#include <string>
#include <stdio.h>

enum class [[nodiscard]] GG : uint32_t {
    W,
    WP,
    NONE
};

// enum class GG : uint32_t {
//     W,
//     WP,
//     NONE
// };


GG wow() {
    return GG::WP;
}

int main() {
    // GG gg = wow();
    // printf("gg=%u\n", static_cast<uint32_t>(gg));

    if (wow() == GG::WP) {
        printf("GG WP\n");
    } else {
        printf("no\n");
    }

    wow();
    return 0;
}