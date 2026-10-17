#include <io_uring_recorder/rotating_recorder.hpp>

#include <io_uring_recorder/io_uring_recorder.hpp>

#include <stdexcept>
#include <utility>

static bool contains_parent_reference(const std::filesystem::path& path) {
    for (const auto& component : path) {
        if (component == "..") {
            return true;
        }
    }
    return false;
}

RotatingRecorder::RotatingRecorder(IoUringRecorder& recorder, RotationConfig config)
    : _recorder(recorder), _config(std::move(config)) {
    if (_config.files_per_directory == 0) {
        throw std::invalid_argument{"files_per_directory must be greater than zero"};
    }
}

void RotatingRecorder::submit(std::filesystem::path logical_directory, std::shared_ptr<const Sample> sample) {
    if (!sample) {
        throw std::invalid_argument{"sample is null"};
    }

    const bool invalid_directory = logical_directory.empty() || logical_directory.is_absolute() ||
                                   contains_parent_reference(logical_directory);
    if (invalid_directory) {
        throw std::invalid_argument{"logical_directory must be a non-empty relative path without parent traversal"};
    }

    _ensure_directory(logical_directory);
    _recorder.submit(_next_file_path(logical_directory), std::move(sample));
}

FlushStats RotatingRecorder::flush() {
    return _recorder.flush();
}

std::filesystem::path RotatingRecorder::_next_file_path(const std::filesystem::path& logical_directory) {
    std::lock_guard lock{_mutex};

    const std::string key = logical_directory.string();
    std::uint64_t& file_index = _file_indexes[key];
    const std::uint64_t slot = file_index % _config.files_per_directory;
    ++file_index;

    return logical_directory / ("sample_" + std::to_string(slot) + ".bin");
}

void RotatingRecorder::_ensure_directory(const std::filesystem::path& logical_directory) {
    const std::string key = logical_directory.string();

    {
        std::lock_guard lock{_mutex};
        if (_created_directories.contains(key)) {
            return;
        }
    }

    std::filesystem::create_directories(_recorder.root_directory() / logical_directory);

    std::lock_guard lock{_mutex};
    _created_directories.insert(key);
}
