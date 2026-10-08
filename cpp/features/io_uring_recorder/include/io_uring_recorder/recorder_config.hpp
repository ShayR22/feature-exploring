#pragma once

#include <cstdint>
#include <filesystem>

struct RecorderConfig {
    std::filesystem::path root_directory;
    std::uint32_t queue_depth{64};
};
