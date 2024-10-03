#include "c_hashmap/c_hashmap.h"
#include <vector>

#define NUM_ITERS (10000)
#define NUM_ITER_PERCENT (NUM_ITERS / 100)

int main(int argc, const char* argv[]) {
    HashMap* hashmap = create_hashmap();

    printf("\nstarting insert operations\n");
    for (int32_t i = 0; i < NUM_ITERS; i++) {
        if (i % NUM_ITER_PERCENT == 0) {
            printf("complete %d%%\n", i / NUM_ITER_PERCENT);
        }
        uint8_t* key = (uint8_t*)(&i);
        size_t key_size = sizeof(i);

        int err = hashmap_insert(hashmap, key, key_size, i);
        if (err) {
            printf("insert failed i=%d\n", i);
            return 1;
        }
    }

    printf("\nstarting get operations\n");
    for (int32_t i = 0; i < NUM_ITERS; i++) {
        if (i % NUM_ITER_PERCENT == 0) {
            printf("complete %u%%\n", i / NUM_ITER_PERCENT);
        }
        uint8_t* key = (uint8_t*)(&i);
        size_t key_size = sizeof(i);

        int value = 0;
        int err = hashmap_get(hashmap, key, key_size, &value);
        if (err) {
            printf("get failed i=%d\n", i);
            return 1;
        }
    }

    return 0;

}