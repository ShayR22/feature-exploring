#include "executor/request_pool.hpp"

#include <limits>
#include <stdexcept>
#include <utility>

RequestPool::RequestPool(const std::size_t capacity) : _requests(capacity), _generations(capacity, 1U) {
    if (capacity == 0) {
        throw std::invalid_argument{"RequestPool capacity must be greater than zero"};
    }

    if (capacity > std::numeric_limits<std::uint32_t>::max()) {
        throw std::invalid_argument{"RequestPool capacity exceeds RequestId slot range"};
    }

    _free_slots.reserve(capacity);
    for (std::size_t index = capacity; index > 0; --index) {
        _free_slots.push_back(static_cast<std::uint32_t>(index - 1));
    }
}

std::size_t RequestPool::capacity() const noexcept {
    return _requests.size();
}

std::size_t RequestPool::available() const noexcept {
    return _free_slots.size();
}

bool RequestPool::empty() const noexcept {
    return _active_count == 0;
}

bool RequestPool::full() const noexcept {
    return _free_slots.empty();
}

RequestId RequestPool::acquire(WriteRequest request) {
    if (_free_slots.empty()) {
        throw std::logic_error{"RequestPool is full"};
    }

    const std::uint32_t slot = _free_slots.back();
    _free_slots.pop_back();
    _requests[slot].emplace(std::move(request));
    ++_active_count;

    return RequestId{
        .slot = slot,
        .generation = _generations[slot],
    };
}

WriteRequest& RequestPool::get(const RequestId id) {
    _validate(id);
    return _requests[id.slot].value();
}

void RequestPool::release(const RequestId id) {
    _validate(id);
    _requests[id.slot].reset();
    --_active_count;

    ++_generations[id.slot];
    if (_generations[id.slot] == 0) {
        _generations[id.slot] = 1;
    }

    _free_slots.push_back(id.slot);
}

void RequestPool::_validate(const RequestId id) const {
    if (id.slot >= _requests.size()) {
        throw std::logic_error{"RequestId slot is out of range"};
    }

    if (_generations[id.slot] != id.generation) {
        throw std::logic_error{"RequestId generation does not match active slot"};
    }

    if (!_requests[id.slot].has_value()) {
        throw std::logic_error{"RequestId refers to an empty slot"};
    }
}
