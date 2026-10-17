#pragma once

#include <array>
#include <concepts>
#include <cstddef>
#include <memory>
#include <new>
#include <stdexcept>
#include <utility>

/**
 * challenge is to create a custom optional class that:
 * 1) allocate on the heap when size of T is bigger than 16 bytes
 * 2) when size is bigger than 16 bytes sizeof(CustomOptional<T>) == 4/8
 * 3) allocate on the stack when it is less or equal to 16 bytes
 * 4) can't assume T is default constructable
 */


constexpr inline std::size_t BIG_TYPES_SIZE_BYTES = 16;

template<typename T>
concept BigAllocationType = sizeof(T) > BIG_TYPES_SIZE_BYTES;

template<typename T>
class CustomOptional;

template<BigAllocationType T>
class CustomOptional<T> {
public:
    CustomOptional() = default;

    explicit CustomOptional(T object) {
        _data_ptr = std::make_unique<T>(std::move(object));
    }

    template<typename ...Args>
        requires std::constructible_from<T, Args...>
    CustomOptional(Args&&... args) {
        _data_ptr = std::make_unique<T>(std::forward<Args>(args)...);
    }

    CustomOptional(const CustomOptional&) = delete;
    CustomOptional& operator=(const CustomOptional&) = delete;

    CustomOptional(CustomOptional&&) = delete;
    CustomOptional& operator=(CustomOptional&&) = delete;

    const T& value() const {
        if (_data_ptr == nullptr) {
            throw std::runtime_error("can't return value on nullopt");
        }
        return *_data_ptr;
    }

    [[nodiscard]] bool has_value() const {
        return *this;
    }

    [[nodiscard]] operator bool() const {
        return _data_ptr != nullptr;
    }

private:
    std::unique_ptr<T> _data_ptr{nullptr};
};


template<typename T>
class CustomOptional {
public:
    CustomOptional() = default;

    explicit CustomOptional(T object) : _is_data_exist{true} {
        ::new (static_cast<void*>(_buffer.data())) T(std::move(object));
    }

    template<typename ...Args>
        requires std::constructible_from<T, Args...>
    CustomOptional(Args&&... args) : _is_data_exist{true} {
        ::new (static_cast<void*>(_buffer.data())) T(std::forward<Args>(args)...);
    }

    ~CustomOptional() {
        if (_is_data_exist) {
            std::destroy_at(std::launder(reinterpret_cast<T*>(_buffer.data())));
        }
    }

    CustomOptional(const CustomOptional&) = delete;
    CustomOptional& operator=(const CustomOptional&) = delete;

    CustomOptional(CustomOptional&&) = delete;
    CustomOptional& operator=(CustomOptional&&) = delete;


    const T& value() const {
        if (!_is_data_exist) {
            throw std::runtime_error("can't return value on nullopt");
        }

        return *std::launder(reinterpret_cast<const T*>(_buffer.data()));
    }

    [[nodiscard]] bool has_value() const {
        return *this;
    }

    [[nodiscard]] operator bool() const {
        return _is_data_exist;
    }


private:
    alignas(T) std::array<std::byte, sizeof(T)> _buffer{};
    bool _is_data_exist{false};
};
