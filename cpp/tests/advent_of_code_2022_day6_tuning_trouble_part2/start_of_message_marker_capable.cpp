#include "start_of_message_marker_capable.hpp"
#include "logger.hpp"
#include <unordered_set>
#include <cassert>
#include <cstdio>

std::vector<uint32_t> SuperSimpleStartMessageMarker::get_start_of_message_marker_indicies(const std::string& message,
                                                                                          uint8_t num_distinct_letters) {
    std::vector<uint32_t> rv{};

    uint32_t i = 0;
    uint32_t iterate_until = message.size() - num_distinct_letters + 1;

    for (uint32_t i = 0; i < iterate_until; i++) {
        std::unordered_set<char> counted_chars{};
        for (uint32_t j = 0; j < num_distinct_letters; j++) {
            char cur_char = message[i + j];
            counted_chars.insert(cur_char);
        }

        if (counted_chars.size() == num_distinct_letters) {
            rv.push_back(i);
        }
    }

    return rv;
}

std::vector<uint32_t> SimpleStartMessageMarker::get_start_of_message_marker_indicies(const std::string& message,
                                                                                     uint8_t num_distinct_letters) {
    std::vector<uint32_t> rv{};

    uint32_t i = 0;
    uint32_t iterate_until = message.size();
    size_t prev_size = 0;
    std::unordered_set<char> counted_chars{};
    do {
        counted_chars.insert(message[i]);
        size_t current_size = counted_chars.size();

        // found duplicate, thus clear cache and continue going on
        if (current_size == prev_size) {
            counted_chars.clear();
            prev_size = 0;
            continue;
        }

        if (current_size == num_distinct_letters) {
            // + 1 because i starts from 0
            uint32_t start_marker_index = i + 1 - num_distinct_letters;

            rv.push_back(start_marker_index);

            // remove the first index of the char we just going to pass by
            counted_chars.erase(message[start_marker_index]);
            prev_size = current_size - 1;
            i++;
            continue;
        }

        i++;

    } while(i < iterate_until);

    return rv;
}


static void _evaluate_char(char c, uint32_t& bitmask) {
    uint8_t bit_index = c - 97;
    uint32_t new_bitmask = bitmask | (1 << bit_index);

    // found duplicate
    if (new_bitmask == bitmask) {
        bitmask = 0;
    } else {
        bitmask = new_bitmask;
    }
}

static inline bool _is_marker_found(uint32_t prev_bitwise) {
    return prev_bitwise != 0;
}

std::vector<uint32_t> ComplexStartMessageMarker::get_start_of_message_marker_indicies(const std::string& message,
                                                                                      uint8_t num_distinct_letters) {
    assert(((void)"current implentation allow until 32 chars", num_distinct_letters <= 32));

    std::vector<uint32_t> rv{};

    uint32_t iterate_until = message.size() - num_distinct_letters + 1;
    uint32_t prev_bitwise = 0;
    int64_t i = 0;
    int64_t j = 0;

    while (i < iterate_until) {
        // advance j to the next window, - 1 as index starts from 0
        j = i + num_distinct_letters - 1;

        if (_is_marker_found(prev_bitwise)) {
            // check the last element for another marker
            char cur_char = message[j];
            _evaluate_char(cur_char, prev_bitwise);
            if (prev_bitwise == 0) {
                // duplicate found advance to the next window
                i += num_distinct_letters - 1;
            }
        } else {
            // iterate over num_distince_letters
            while (j >= i) {
                char cur_char = message[j];
                _evaluate_char(cur_char, prev_bitwise);
                if (prev_bitwise == 0) {
                    // duplicate found advance to the duplication index
                    i = j;
                    break;
                }
                j--;
            }
        }

        // found match
        if (_is_marker_found(prev_bitwise)) {
            uint32_t start_marker_index = i;
            rv.push_back(start_marker_index);

            // clear bit of start_marker_index
            char start_marker_char = message[start_marker_index];
            uint8_t bit_index = start_marker_char - 97;
            prev_bitwise &= ~(1 << bit_index);
            i++;
        }
    }

    return rv;
}