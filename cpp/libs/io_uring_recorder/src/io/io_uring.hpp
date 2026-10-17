#pragma once

#include "executor/request_id.hpp"
#include "io/completion.hpp"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <span>

struct io_uring;

class IoUring {
public:
    [[nodiscard]] static std::optional<IoUring> from(std::uint32_t queue_depth);

    ~IoUring();

    IoUring(const IoUring&) = delete;
    IoUring& operator=(const IoUring&) = delete;
    IoUring(IoUring&& other) noexcept;
    IoUring& operator=(IoUring&&) = delete;

    void prepare_open(RequestId id, const std::filesystem::path& path);
    void prepare_write(RequestId id, int fd, std::span<const std::byte> data, std::uint64_t offset);
    void prepare_close(RequestId id, int fd);
    void submit();

    [[nodiscard]] Completion wait();
    [[nodiscard]] std::optional<Completion> peek();

private:
    explicit IoUring(std::unique_ptr<io_uring> ring) noexcept;

    std::unique_ptr<io_uring> _ring;
};
