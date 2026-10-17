

#pragma once

#include <memory>


class ArenaAllocator;

class Allocation {
public:
    Allocation(ArenaAllocator& arena, uint8_t* buffer, uint32_t size);
    uint8_t* get();

    ~Allocation();

private:
    ArenaAllocator& _arena;
    uint8_t* _buffer;
    uint32_t _size;
};

class ArenaAllocator {
public:
    ArenaAllocator(uint32_t size);
    Allocation allocate(uint32_t size);

private:
    friend Allocation::~Allocation();
    void deallocate(uint32_t size);

    std::unique_ptr<uint8_t[]> _buffer;
    uint32_t _size{0};
    uint32_t _position{0};
};