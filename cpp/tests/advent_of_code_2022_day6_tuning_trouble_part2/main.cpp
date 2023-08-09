// #include "start_of_message_marker_capable.hpp"
#include "logger.hpp"
#include <string>
#include <cstdint>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <cstdio>
#include <span>
#include <optional>


std::optional<uint32_t> super_simple_get_marker_index(const std::string& message, uint8_t num_distinct_letters) {
    uint32_t i = 0;
    uint32_t iterate_until = message.size() - num_distinct_letters + 1;

    for (uint32_t i = 0; i < iterate_until; i++) {
        std::unordered_set<char> counted_chars{};
        for (uint32_t j = 0; j < num_distinct_letters; j++) {
            char cur_char = message[i + j];
            counted_chars.insert(cur_char);
        }

        if (counted_chars.size() == num_distinct_letters) {
            return i;
        }
    }

    return std::nullopt;
}

static uint32_t smart_simple_find_marker(std::span<const char> small_chars) {
    std::unordered_set<char> distinct_chars{};
    uint32_t num_chars = small_chars.size();
    uint32_t prev_chars_count = 0;
    for(int32_t i = num_chars - 1; i >= 0; i--) {
        distinct_chars.insert(small_chars[i]);
        uint32_t current_chars_count = distinct_chars.size();

        // found duplicate
        if (current_chars_count == prev_chars_count) {
            return i + 1;
        }

        prev_chars_count = current_chars_count;
    }

    // found marker meaning advance marker by one
    return 0;
}

std::optional<uint32_t> smart_get_marker_index(const std::string& message, uint8_t num_distinct_letters) {
    uint32_t iterate_until = message.size() - num_distinct_letters + 1;
    uint32_t i = 0;
    while (i < iterate_until) {
        const char* start_message = message.data() + i;
        std::span<const char> char_views{start_message, num_distinct_letters};
        uint32_t duplicate_index = smart_simple_find_marker(char_views);
        if (duplicate_index == 0) {
            return i;
        }

        i += duplicate_index;
    }

    return std::nullopt;;
}

// std::optional<uint32_t> god_mode_get_marker_index(const std::string& message, uint8_t num_distinct_letters) {
//     assert(((void)"current implentation allow until 32 chars", num_distinct_letters <= 32));

//     std::vector<uint32_t> rv{};

//     uint32_t iterate_until = message.size() - num_distinct_letters + 1;
//     uint32_t prev_bitwise = 0;
//     int64_t i = 0;
//     int64_t j = 0;

//     while (i < iterate_until) {
//         // advance j to the next window, - 1 as index starts from 0
//         j = i + num_distinct_letters - 1;

//         if (_is_marker_found(prev_bitwise)) {
//             // check the last element for another marker
//             char cur_char = message[j];
//             _evaluate_char(cur_char, prev_bitwise);
//             if (prev_bitwise == 0) {
//                 // duplicate found advance to the next window
//                 i += num_distinct_letters - 1;
//             }
//         } else {
//             // iterate over num_distince_letters
//             while (j >= i) {
//                 char cur_char = message[j];
//                 _evaluate_char(cur_char, prev_bitwise);
//                 if (prev_bitwise == 0) {
//                     // duplicate found advance to the duplication index
//                     i = j;
//                     break;
//                 }
//                 j--;
//             }
//         }

//         // found match
//         if (_is_marker_found(prev_bitwise)) {
//             uint32_t start_marker_index = i;
//             rv.push_back(start_marker_index);

//             // clear bit of start_marker_index
//             char start_marker_char = message[start_marker_index];
//             uint8_t bit_index = start_marker_char - 97;
//             prev_bitwise &= ~(1 << bit_index);
//             i++;
//         }
//     }

//     return rv;
// }


static std::string generate_random_small_letters(uint32_t num_letters, uint32_t num_distinct_letters) {
    assert(((void)"num_letters must be bigger than num_distinct_letters", num_letters >= num_distinct_letters));
    LOG("num_leters=[%u] num_distinct_letters=[%u]", num_letters, num_distinct_letters);
    std::string rv{};
    rv.reserve(num_letters + 1);

    // generature random letters
    for (uint32_t i = 0; i < num_letters; i++) {
        rv += (std::rand() % 26) + 97;
    }

    for (uint32_t i = 0; i < num_letters - num_distinct_letters; i++) {
        const char* start_message = rv.data() + i;
        std::span<const char> char_views{start_message, num_distinct_letters};
        int marker_index = smart_simple_find_marker(char_views);
        if (marker_index == 0) {
            rv[i + num_distinct_letters - 1] = rv[i];
        }
    }

    // generate num_distinct_letters based on the last char added
    uint32_t last_char_copy_index = rv.size() - num_distinct_letters - 1;
    char last_char = rv[last_char_copy_index];
    for (uint32_t i = 0; i < num_distinct_letters; i++) {
        uint32_t set_index = rv.size() - num_distinct_letters + i;
        char set_char_to = ((last_char - 97 + i) % 26) + 97;
        rv[set_index] = set_char_to;
    }

    LOG("rv=[%s]", rv.c_str());
    return rv;
}

static void solve(auto&& solver, const std::string& message, uint8_t num_letters) {
    LOG("message=[%s] num_letters=[%u]", message.c_str(), num_letters);
    std::optional<uint32_t> opt_index = solver(message, num_letters);

    if (opt_index) {
        LOG("index=[%u]", *opt_index);
    } else {
        LOG("no index was found");
    }
}


int main(int argc, const char* argv[]) {
    uint32_t num_letters = 27;
    uint8_t num_distinct_letters = 20;
    std::string message = generate_random_small_letters(num_letters, num_distinct_letters);
    LOG("message=[%s]\n", message.c_str());

    solve(super_simple_get_marker_index  , message, num_letters);
    solve(smart_get_marker_index        , message, num_letters);
    // solve(simple_get_marker_index        , message, num_letters);
    // solve(god_mode_get_marker_index      , message, num_letters);

    return 0;
}
