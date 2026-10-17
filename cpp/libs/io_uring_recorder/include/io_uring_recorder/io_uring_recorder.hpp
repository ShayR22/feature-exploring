#pragma once

#include <io_uring_recorder/flush_stats.hpp>
#include <io_uring_recorder/recorder_config.hpp>
#include <io_uring_recorder/sample.hpp>

#include <condition_variable>
#include <deque>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

class IoUringExecutor;

class IoUringRecorder {
public:
    explicit IoUringRecorder(RecorderConfig config);
    ~IoUringRecorder();

    IoUringRecorder(const IoUringRecorder&) = delete;
    IoUringRecorder& operator=(const IoUringRecorder&) = delete;
    IoUringRecorder(IoUringRecorder&&) = delete;
    IoUringRecorder& operator=(IoUringRecorder&&) = delete;

    void submit(std::filesystem::path relative_file_path, std::shared_ptr<const Sample> sample);
    [[nodiscard]] FlushStats flush();
    [[nodiscard]] const std::filesystem::path& root_directory() const noexcept;

private:
    void _worker_loop() noexcept;
    void _feed_executor();
    void _record_error(std::string message);

    RecorderConfig _config;
    std::unique_ptr<IoUringExecutor> _executor;
    std::jthread _worker;

    std::mutex _mutex;
    std::condition_variable _work_available;
    std::deque<std::pair<std::filesystem::path, std::shared_ptr<const Sample>>> _requests;

    bool _accepting{true};
    bool _flushed{false};
    std::string _first_error;
};
