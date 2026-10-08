#pragma once

#include <io_uring_recorder/flush_stats.hpp>
#include <io_uring_recorder/rotation_config.hpp>
#include <io_uring_recorder/sample.hpp>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

class IoUringRecorder;

class RotatingRecorder {
public:
    RotatingRecorder(IoUringRecorder& recorder, RotationConfig config);

    void submit(std::filesystem::path logical_directory, std::shared_ptr<const Sample> sample);
    [[nodiscard]] FlushStats flush();

private:
    [[nodiscard]] std::filesystem::path _next_file_path(const std::filesystem::path& logical_directory);
    void _ensure_directory(const std::filesystem::path& logical_directory);

    IoUringRecorder& _recorder;
    RotationConfig _config;
    std::mutex _mutex;
    std::unordered_map<std::string, std::uint64_t> _file_indexes;
    std::unordered_set<std::string> _created_directories;
};
