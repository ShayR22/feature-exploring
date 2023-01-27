#include <vector>
#include <algorithm>
#include <concepts>
#include <compare>
#include <random>

#include "../func_measuring/tester.hpp"
#include <stdint.h>
#include <unordered_map>
#include <unordered_set>

// A multiplier that is used when calculating the group value for the hashboard
// group (ASIC_HASHBOARD_x_ADDRESS) and the string-in-hashboard group
// (ASIC_HASHBOARD_x_STRING_y_ADDRESS) in order to avoid collisions.
static constexpr uint8_t HASHBOARD_INDEX_MULTIPLIER = 16;
static constexpr uint16_t GROUP_MULTICAST1 = 500;
static constexpr uint16_t GROUP_BROADCAST = 0xFFFB;

using HermesGroupSet = std::unordered_set<uint16_t>;
using GroupToAsicIds = std::unordered_map<uint16_t, HermesGroupSet>;
using AsicIdToGroups = std::unordered_map<uint16_t, HermesGroupSet>;


// Map groups to asic ids
GroupToAsicIds _group_to_asic_ids{};
// Map asic ids to which group each one belongs to
AsicIdToGroups _asic_id_to_groups{};


void _add_asic_to_group_mapping(uint16_t asic_id, uint16_t hb_id, uint16_t str_id) {
    // GROUP(x * HASHBOARD_INDEX_MULTIPLIER) + 1
    const uint16_t ASIC_HASHBOARD_x_ADDRESS = GROUP_MULTICAST1 + hb_id * HASHBOARD_INDEX_MULTIPLIER;

    _asic_id_to_groups[asic_id].insert(ASIC_HASHBOARD_x_ADDRESS);
    _group_to_asic_ids[ASIC_HASHBOARD_x_ADDRESS].insert(asic_id);

    // GROUP(x * HASHBOARD_INDEX_MULTIPLIER + (y + 1)) + 1
    const uint16_t ASIC_HASHBOARD_x_STRING_y_ADDRESS = GROUP_MULTICAST1 + hb_id * HASHBOARD_INDEX_MULTIPLIER + str_id + 1;

    _asic_id_to_groups[asic_id].insert(ASIC_HASHBOARD_x_STRING_y_ADDRESS);
    _group_to_asic_ids[ASIC_HASHBOARD_x_STRING_y_ADDRESS].insert(asic_id);

    // Map the group of the asic to itself
    _group_to_asic_ids[asic_id].insert(asic_id);

    // Map the broadcast group to include the asic
    _group_to_asic_ids[GROUP_BROADCAST].insert(asic_id);
}

int get_groups_by_asic_id(uint16_t asic_id, HermesGroupSet& group_set) {
    const auto& iter = _asic_id_to_groups.find(asic_id);
    if (iter == _asic_id_to_groups.end()) {
        printf("asic_id=[%u] not found in _group_to_asic_ids map\n", asic_id);
        return -1;
    }

    group_set = iter->second;
    return 0;
}

int get_asic_ids_by_group(uint16_t group, HermesGroupSet& group_set) {
    const auto& iter = _group_to_asic_ids.find(group);
    if (iter == _group_to_asic_ids.end()) {
        printf("group=[%u] not found in _asic_id_to_groups map", group);
        return -1;
    }

    group_set = iter->second;
    return 0;
}

const GroupToAsicIds& get_groups_mapping() {
    return _group_to_asic_ids;
}

const AsicIdToGroups& get_asic_ids_mapping() {
    return _asic_id_to_groups;
}


void init_mapping() {
    uint16_t asic_id = 1;
    for (auto hb_id = 0; hb_id < 4; hb_id++) {
        for (auto str_id = 0; str_id < 2; str_id++) {
            for (auto grp_id = 0; grp_id < 5; grp_id++) {
                auto num_asics_in_group = grp_id == 0 ? 2 : 4;
                for (auto aid = 0; aid < num_asics_in_group; aid++) {
                    _add_asic_to_group_mapping(asic_id, hb_id, str_id);
                    asic_id++;
                }
            }
        }
    }
}

void print(const char* name, const auto& container) {
    printf("%s:\n[", name);
    for (const auto& item: container) {
        printf("%s, ", std::to_string(item).c_str());
    }
    printf("]\n");
}

int main() {
    init_mapping();

    auto get_bcast_set_lambda = []() {
        HermesGroupSet asics_set{};
        int err = get_asic_ids_by_group(GROUP_BROADCAST, asics_set);
    };

    auto get_unicast_set_lambda = []() {
        HermesGroupSet asics_set{};
        int err = get_asic_ids_by_group(1, asics_set);
    };

    uint32_t nano_took = measure_avg_execution_time_nano("get_unicast_set_lambda", get_unicast_set_lambda);
    printf("nano_took=%u\n", nano_took);
    nano_took =measure_avg_execution_time_nano("get_bcast_set_lambda", get_bcast_set_lambda);
    printf("nano_took=%u\n", nano_took);
    return 0;
}