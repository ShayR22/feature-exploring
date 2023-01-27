#include <thread>
#include <stdio.h>
#include <map>
#include <unordered_map>
#include <array>
#include "tester.hpp"
#include <stdexcept>
#include <algorithm>

static constexpr uint32_t KB_SIZE_BYTES = 1024;
static constexpr uint32_t MB_SIZE_BYTES = KB_SIZE_BYTES * KB_SIZE_BYTES;


static uint32_t size_mb = 1;
static uint32_t num_calls = 0;


template<typename Key, typename Value, std::size_t Size>
struct Map {
    std::array<std::pair<Key, Value>, Size> data;

    constexpr Value at(const Key& key) const {
        const auto itr = std::find_if(begin(data), end(data), [&key](const auto& v) {return v.first == key; });

        if (itr != end(data)) {
            return itr->second;
        }

        throw std::range_error("Key not found");
    }
};


static constexpr std::array<std::pair<int, int>, 10> my_values{
   {{3211, 123},
    {12834, 9321},
    {11111, 2},
    {329111, 9000},
    {191243, 877},
    {11, 122},
    {0, 1230},
    {2148924, 977},
    {243, 120311},
    {1194, 812347}}
};

static constexpr auto array_map = Map<int, int, my_values.size()>{{my_values}};


static constexpr std::array<int, 10> keys = {
    3211,
    12834,
    11111,
    329111,
    191243,
    11,
    0,
    2148924,
    243,
    1194
};

static std::map<int, int> my_values_map {
    {3211, 123},
    {12834, 9321},
    {11111, 2},
    {329111, 9000},
    {191243, 877},
    {11, 122},
    {0, 1230},
    {2148924, 977},
    {243, 120311},
    {1194, 812347}
};

int lookup_value(const int key) {
    static constexpr auto map = Map<int, int, my_values.size()>{{my_values}};

    return map.at(key);
}

template<typename T>
static void test_generic_map(T& my_map, const std::string& map_type_str) {

    static constexpr uint8_t MAP_ENTRY_SIZE = 12;
    uint32_t num_elements = size_mb * MB_SIZE_BYTES / MAP_ENTRY_SIZE;
    printf("map type %s: num_elemnets = %u, size_mb = %u\n", map_type_str.c_str(), num_elements, size_mb);


    auto get_random_key = [&]() -> int{
        return keys[rand() % keys.size()];
    };

    std::vector<int> random_keys{};
    random_keys.resize(num_elements);
    for (int i = 0 ; i < num_elements; i++) {
        random_keys.push_back(get_random_key());
    }

    auto sum_random_elements = [&]() -> uint32_t {
        uint32_t sum = 0;
        for (const auto& key: random_keys ) {
            sum += my_map.at(key);
        }
        return sum;
    };

    if (num_calls == 0) {
        measure_avg_execution_time(sum_random_elements);
    } else {
        measure_avg_execution_time(sum_random_elements, num_calls);
    }
}

static void test_std_map() {
    test_generic_map(my_values_map, "std::map");
}

static void test_array_map() {
    test_generic_map(array_map, "array_map");
}


int main(int argc, const char* argv[]) {

    uint32_t hard_function_avg_exec_time_ms = -1;


    if (argc > 1) {
        num_calls = atoi(argv[1]);
    }

    if (argc > 2) {
        size_mb = atoi(argv[2]);
    }


    test_array_map();
    test_std_map();



    constexpr auto value = array_map.at(243);

    printf("value = %d\n", value);

    return 0;
}

