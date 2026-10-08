#pragma once

#include "executor/request_id.hpp"
#include "executor/request_pool.hpp"
#include "executor/write_request.hpp"
#include "io/completion.hpp"
#include "io/io_uring.hpp"

#include <io_uring_recorder/sample.hpp>

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>

class IoUringExecutor {
public:
    explicit IoUringExecutor(std::uint32_t queue_depth);
    ~IoUringExecutor();

    IoUringExecutor(const IoUringExecutor&) = delete;
    IoUringExecutor& operator=(const IoUringExecutor&) = delete;
    IoUringExecutor(IoUringExecutor&&) = delete;
    IoUringExecutor& operator=(IoUringExecutor&&) = delete;

    [[nodiscard]] std::size_t capacity() const noexcept;
    [[nodiscard]] std::size_t available() const noexcept;
    [[nodiscard]] bool has_active() const noexcept;
    [[nodiscard]] bool try_submit(std::filesystem::path file_path, std::shared_ptr<const Sample> sample);

    void wait_for_capacity();
    void wait_for_progress();
    void flush();

private:
    void _process_one_blocking_completion();
    void _drain_ready_completions();
    void _process_completion(Completion completion);
    void _on_open_complete(RequestId id, WriteRequest& request, int result);
    void _on_write_complete(RequestId id, WriteRequest& request, int result);
    void _on_close_complete(RequestId id, WriteRequest& request, int result);
    void _prepare_open(RequestId id, WriteRequest& request);
    void _prepare_write(RequestId id, WriteRequest& request);
    void _prepare_close(RequestId id, WriteRequest& request);
    void _record_error(std::string message);

    IoUring _ring;
    RequestPool _request_pool;
    std::string _first_error;
};
