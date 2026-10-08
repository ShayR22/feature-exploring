#include <io_uring_recorder/io_uring_recorder.hpp>

#include "executor/io_uring_executor.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <chrono>
#include <stdexcept>
#include <system_error>
#include <utility>

using Clock = std::chrono::steady_clock;

static double seconds_between(const Clock::time_point start, const Clock::time_point end) {
    return std::chrono::duration<double>(end - start).count();
}

static void sync_file_system(const std::filesystem::path& root_directory) {
    const int fd = ::open(root_directory.c_str(), O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    if (fd < 0) {
        throw std::system_error{errno, std::generic_category(), "Failed to open recording root directory for sync"};
    }

    if (::syncfs(fd) != 0) {
        const int error = errno;
        static_cast<void>(::close(fd));
        throw std::system_error{error, std::generic_category(), "syncfs failed"};
    }

    if (::close(fd) != 0) {
        throw std::system_error{errno, std::generic_category(), "Failed to close recording root directory"};
    }
}

static bool contains_parent_reference(const std::filesystem::path& path) {
    for (const auto& component : path) {
        if (component == "..") {
            return true;
        }
    }
    return false;
}

IoUringRecorder::IoUringRecorder(RecorderConfig config)
    : _config(std::move(config)), _executor(std::make_unique<IoUringExecutor>(_config.queue_depth)) {
    if (_config.root_directory.empty()) {
        throw std::invalid_argument{"root_directory must not be empty"};
    }

    std::filesystem::create_directories(_config.root_directory);
    _worker = std::jthread([this] { _worker_loop(); });
}

IoUringRecorder::~IoUringRecorder() {
    try {
        if (!_flushed) {
            static_cast<void>(flush());
        }
    } catch (...) {
    }
}

void IoUringRecorder::submit(std::filesystem::path relative_file_path, std::shared_ptr<const Sample> sample) {
    if (!sample) {
        throw std::invalid_argument{"sample is null"};
    }
    const bool invalid_path =
        relative_file_path.empty() || relative_file_path.is_absolute() || contains_parent_reference(relative_file_path);
    if (invalid_path) {
        throw std::invalid_argument{"file path must be a non-empty relative path without parent traversal"};
    }

    {
        std::lock_guard lock{_mutex};
        if (!_accepting) {
            throw std::runtime_error{"Recorder is no longer accepting samples"};
        }
        _requests.emplace_back(std::move(relative_file_path), std::move(sample));
    }

    _work_available.notify_one();
}

FlushStats IoUringRecorder::flush() {
    {
        std::lock_guard lock{_mutex};
        if (_flushed) {
            return {};
        }
        _accepting = false;
    }

    _work_available.notify_all();

    const auto wait_start = Clock::now();
    if (_worker.joinable()) {
        _worker.join();
    }
    const auto wait_end = Clock::now();

    std::string error;
    {
        std::lock_guard lock{_mutex};
        error = _first_error;
    }
    if (!error.empty()) {
        throw std::runtime_error{error};
    }

    const auto sync_start = Clock::now();
    sync_file_system(_config.root_directory);
    const auto sync_end = Clock::now();

    {
        std::lock_guard lock{_mutex};
        _flushed = true;
    }

    return FlushStats{
        .wait_for_async_io_seconds = seconds_between(wait_start, wait_end),
        .sync_seconds = seconds_between(sync_start, sync_end),
    };
}

const std::filesystem::path& IoUringRecorder::root_directory() const noexcept {
    return _config.root_directory;
}

void IoUringRecorder::_worker_loop() noexcept {
    try {
        while (true) {
            _feed_executor();

            std::unique_lock lock{_mutex};
            if (!_accepting && _requests.empty()) {
                lock.unlock();
                _executor->flush();
                return;
            }

            if (!_requests.empty()) {
                lock.unlock();
                _executor->wait_for_capacity();
                continue;
            }

            if (_executor->has_active()) {
                lock.unlock();
                _executor->wait_for_progress();
                continue;
            }

            _work_available.wait(lock, [this] { return !_requests.empty() || !_accepting; });
        }
    } catch (const std::exception& error) {
        _record_error(error.what());
    } catch (...) {
        _record_error("Unknown recorder worker failure");
    }
}

void IoUringRecorder::_feed_executor() {
    while (_executor->available() > 0) {
        std::pair<std::filesystem::path, std::shared_ptr<const Sample>> request;

        {
            std::lock_guard lock{_mutex};
            if (_requests.empty()) {
                return;
            }
            request = std::move(_requests.front());
            _requests.pop_front();
        }

        const std::filesystem::path file_path = _config.root_directory / request.first;
        if (!_executor->try_submit(file_path, std::move(request.second))) {
            throw std::logic_error{"Executor reported capacity but rejected the request"};
        }
    }
}

void IoUringRecorder::_record_error(std::string message) {
    std::lock_guard lock{_mutex};
    if (_first_error.empty()) {
        _first_error = std::move(message);
    }
    _accepting = false;
    _work_available.notify_all();
}
