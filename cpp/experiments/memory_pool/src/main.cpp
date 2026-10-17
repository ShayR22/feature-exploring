#include "memory_pool.hpp"
#include <vector>
#include <cstdint>

#include <cstdio>
#include <memory>
#include <utility>

struct Location {
    int x;
    int y;
    int z;
};

static constexpr uint32_t BIG_DATA_SIZE = 128;

struct BigData {
    uint8_t big_data[BIG_DATA_SIZE];
};

static constexpr uint32_t BLOCK_SIZE = sizeof(Location);
static constexpr uint32_t NUM_BLOCKS = 10;
static constexpr uint32_t MEANING_OF_LIFE = 42;

int main() {
    MemoryPool pool(BLOCK_SIZE, NUM_BLOCKS);

    {
        std::vector<PoolUniquePtr<uint32_t>> my_ptrs{};
        for (uint32_t i = 0; i < NUM_BLOCKS; i++) {
            PoolUniquePtr<uint32_t> ptr = pool.allocate<uint32_t>(i * NUM_BLOCKS);
            my_ptrs.emplace_back(std::move(ptr));
        }

        for (const auto& ptr: my_ptrs) {
            printf("%u\n", *ptr);
        }
    }


    PoolUniquePtr<uint32_t> ptr = pool.allocate<uint32_t>(MEANING_OF_LIFE);

    pool.allocate<BigData>();




    return 0;
}
