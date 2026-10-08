#pragma once

#include "executor/request_stage.hpp"

#include <io_uring_recorder/sample.hpp>

#include <cstddef>
#include <filesystem>
#include <memory>
#include <optional>

struct WriteRequest {
    std::filesystem::path file_path;
    std::shared_ptr<const Sample> sample;
    std::optional<int> fd;
    std::size_t offset{0};
    RequestStage stage{RequestStage::Open};
};
