#include "executor/io_uring_executor.hpp"

#include <algorithm>
#include <limits>
#include <optional>
#include <span>
#include <stdexcept>
#include <system_error>
#include <utility>

static IoUring create_io_uring(const std::uint32_t queue_depth) {
    std::optional<IoUring> ring = IoUring::from(queue_depth);
    if (!ring.has_value()) {
        throw std::runtime_error{"Failed to initialize io_uring"};
    }
    return std::move(*ring);
}

IoUringExecutor::IoUringExecutor(const std::uint32_t queue_depth)
    : _ring(create_io_uring(queue_depth)), _request_pool(queue_depth) {
}

IoUringExecutor::~IoUringExecutor() {
    try {
        flush();
    } catch (...) {
    }
}

std::size_t IoUringExecutor::capacity() const noexcept {
    return _request_pool.capacity();
}

std::size_t IoUringExecutor::available() const noexcept {
    return _request_pool.available();
}

bool IoUringExecutor::has_active() const noexcept {
    return !_request_pool.empty();
}

bool IoUringExecutor::try_submit(std::filesystem::path file_path, std::shared_ptr<const Sample> sample) {
    if (!sample) {
        throw std::invalid_argument{"sample is null"};
    }

    if (_request_pool.full()) {
        return false;
    }

    const RequestId id = _request_pool.acquire(WriteRequest{
        .file_path = std::move(file_path),
        .sample = std::move(sample),
        .fd = std::nullopt,
        .offset = 0,
        .stage = RequestStage::Open,
    });

    try {
        _prepare_open(id, _request_pool.get(id));
    } catch (...) {
        _request_pool.release(id);
        throw;
    }

    _ring.submit();
    return true;
}

void IoUringExecutor::wait_for_capacity() {
    while (_request_pool.full()) {
        _process_one_blocking_completion();
        _drain_ready_completions();
        _ring.submit();
    }
}

void IoUringExecutor::wait_for_progress() {
    if (_request_pool.empty()) {
        return;
    }

    _process_one_blocking_completion();
    _drain_ready_completions();
    _ring.submit();
}

void IoUringExecutor::flush() {
    while (!_request_pool.empty()) {
        wait_for_progress();
    }

    if (!_first_error.empty()) {
        throw std::runtime_error{_first_error};
    }
}

void IoUringExecutor::_process_one_blocking_completion() {
    _process_completion(_ring.wait());
}

void IoUringExecutor::_drain_ready_completions() {
    while (true) {
        const std::optional<Completion> completion = _ring.peek();
        if (!completion.has_value()) {
            return;
        }
        _process_completion(*completion);
    }
}

void IoUringExecutor::_process_completion(const Completion completion) {
    const RequestId id = completion.request_id;
    WriteRequest& request = _request_pool.get(id);

    switch (request.stage) {
    case RequestStage::Open:
        _on_open_complete(id, request, completion.result);
        return;
    case RequestStage::Write:
        _on_write_complete(id, request, completion.result);
        return;
    case RequestStage::Close:
        _on_close_complete(id, request, completion.result);
        return;
    }

    throw std::logic_error{"Unknown WriteRequest stage"};
}

void IoUringExecutor::_on_open_complete(const RequestId id, WriteRequest& request, const int result) {
    if (result < 0) {
        _record_error("open failed for " + request.file_path.string() + ": " +
                      std::error_code{-result, std::generic_category()}.message());
        _request_pool.release(id);
        return;
    }

    request.fd = result;
    if (request.sample->data.empty()) {
        _prepare_close(id, request);
        return;
    }

    _prepare_write(id, request);
}

void IoUringExecutor::_on_write_complete(const RequestId id, WriteRequest& request, const int result) {
    if (result < 0) {
        _record_error("write failed for " + request.file_path.string() + ": " +
                      std::error_code{-result, std::generic_category()}.message());
        _prepare_close(id, request);
        return;
    }

    if (result == 0) {
        _record_error("write completed with 0 bytes for " + request.file_path.string());
        _prepare_close(id, request);
        return;
    }

    request.offset += static_cast<std::size_t>(result);
    if (request.offset < request.sample->data.size()) {
        _prepare_write(id, request);
        return;
    }

    _prepare_close(id, request);
}

void IoUringExecutor::_on_close_complete(const RequestId id, WriteRequest& request, const int result) {
    if (result < 0) {
        _record_error("close failed for " + request.file_path.string() + ": " +
                      std::error_code{-result, std::generic_category()}.message());
    }

    request.fd.reset();
    _request_pool.release(id);
}

void IoUringExecutor::_prepare_open(const RequestId id, WriteRequest& request) {
    request.stage = RequestStage::Open;
    _ring.prepare_open(id, request.file_path);
}

void IoUringExecutor::_prepare_write(const RequestId id, WriteRequest& request) {
    request.stage = RequestStage::Write;

    const auto& data = request.sample->data;
    const std::size_t remaining = data.size() - request.offset;
    const std::size_t write_size = std::min<std::size_t>(remaining, std::numeric_limits<unsigned>::max());
    const std::span<const std::byte> write_data{data.data() + request.offset, write_size};

    _ring.prepare_write(id, request.fd.value(), write_data, request.offset);
}

void IoUringExecutor::_prepare_close(const RequestId id, WriteRequest& request) {
    request.stage = RequestStage::Close;
    _ring.prepare_close(id, request.fd.value());
}

void IoUringExecutor::_record_error(std::string message) {
    if (_first_error.empty()) {
        _first_error = std::move(message);
    }
}
