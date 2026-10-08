#pragma once

#include "executor/request_id.hpp"
#include "executor/write_request.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

class RequestPool {
public:
    explicit RequestPool(std::size_t capacity);

    [[nodiscard]] std::size_t capacity() const noexcept;
    [[nodiscard]] std::size_t available() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] bool full() const noexcept;

    [[nodiscard]] RequestId acquire(WriteRequest request);
    [[nodiscard]] WriteRequest& get(RequestId id);
    void release(RequestId id);

private:
    void _validate(RequestId id) const;

    std::vector<std::optional<WriteRequest>> _requests;
    std::vector<std::uint32_t> _generations;
    std::vector<std::uint32_t> _free_slots;
    std::size_t _active_count{0};
};
