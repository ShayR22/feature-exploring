#include "io/io_uring.hpp"

#include <liburing.h>

#include <fcntl.h>

#include <cerrno>
#include <limits>
#include <stdexcept>
#include <system_error>
#include <utility>

std::optional<IoUring> IoUring::from(const std::uint32_t queue_depth) {
    if (queue_depth == 0) {
        return std::nullopt;
    }

    auto ring = std::make_unique<io_uring>();
    const int result = ::io_uring_queue_init(queue_depth, ring.get(), 0);
    if (result < 0) {
        return std::nullopt;
    }

    return IoUring{std::move(ring)};
}

IoUring::IoUring(std::unique_ptr<io_uring> ring) noexcept : _ring(std::move(ring)) {
}

IoUring::IoUring(IoUring&& other) noexcept : _ring(std::move(other._ring)) {
}

IoUring::~IoUring() {
    if (_ring) {
        ::io_uring_queue_exit(_ring.get());
    }
}

void IoUring::prepare_open(const RequestId id, const std::filesystem::path& path) {
    io_uring_sqe* const sqe = ::io_uring_get_sqe(_ring.get());
    if (sqe == nullptr) {
        throw std::runtime_error{"io_uring submission queue is full"};
    }

    ::io_uring_prep_openat(sqe, AT_FDCWD, path.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
    sqe->user_data = id.raw();
}

void IoUring::prepare_write(
    const RequestId id,
    const int fd,
    const std::span<const std::byte> data,
    const std::uint64_t offset) {
    if (data.size() > std::numeric_limits<unsigned>::max()) {
        throw std::invalid_argument{"Single io_uring write exceeds unsigned length range"};
    }

    io_uring_sqe* const sqe = ::io_uring_get_sqe(_ring.get());
    if (sqe == nullptr) {
        throw std::runtime_error{"io_uring submission queue is full"};
    }

    ::io_uring_prep_write(sqe, fd, data.data(), static_cast<unsigned>(data.size()), static_cast<off_t>(offset));
    sqe->user_data = id.raw();
}

void IoUring::prepare_close(const RequestId id, const int fd) {
    io_uring_sqe* const sqe = ::io_uring_get_sqe(_ring.get());
    if (sqe == nullptr) {
        throw std::runtime_error{"io_uring submission queue is full"};
    }

    ::io_uring_prep_close(sqe, fd);
    sqe->user_data = id.raw();
}

void IoUring::submit() {
    if (::io_uring_sq_ready(_ring.get()) == 0) {
        return;
    }

    int result = 0;
    do {
        result = ::io_uring_submit(_ring.get());
    } while (result == -EINTR);

    if (result < 0) {
        throw std::system_error{-result, std::generic_category(), "io_uring_submit failed"};
    }
}

Completion IoUring::wait() {
    io_uring_cqe* cqe = nullptr;

    int result = 0;
    do {
        result = ::io_uring_wait_cqe(_ring.get(), &cqe);
    } while (result == -EINTR);

    if (result < 0) {
        throw std::system_error{-result, std::generic_category(), "io_uring_wait_cqe failed"};
    }

    const Completion completion{
        .request_id = RequestId::from_raw(cqe->user_data),
        .result = cqe->res,
    };

    ::io_uring_cqe_seen(_ring.get(), cqe);
    return completion;
}

std::optional<Completion> IoUring::peek() {
    io_uring_cqe* cqe = nullptr;
    const int result = ::io_uring_peek_cqe(_ring.get(), &cqe);

    if (result == -EAGAIN) {
        return std::nullopt;
    }

    if (result < 0) {
        throw std::system_error{-result, std::generic_category(), "io_uring_peek_cqe failed"};
    }

    const Completion completion{
        .request_id = RequestId::from_raw(cqe->user_data),
        .result = cqe->res,
    };

    ::io_uring_cqe_seen(_ring.get(), cqe);
    return completion;
}
