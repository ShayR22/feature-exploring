#pragma once

#include <cstdint>

struct RequestId {
    std::uint32_t slot{0};
    std::uint32_t generation{0};

    [[nodiscard]] std::uint64_t raw() const noexcept {
        return (static_cast<std::uint64_t>(generation) << 32U) | static_cast<std::uint64_t>(slot);
    }

    [[nodiscard]] static RequestId from_raw(const std::uint64_t value) noexcept {
        return RequestId{
            .slot = static_cast<std::uint32_t>(value & 0xFFFFFFFFULL),
            .generation = static_cast<std::uint32_t>(value >> 32U),
        };
    }
};
