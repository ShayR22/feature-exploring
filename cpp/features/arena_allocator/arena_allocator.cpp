#include "arena_allocator.hpp"
#include <cstdio>
#include <stdexcept>

Allocation::Allocation(ArenaAllocator& arena, uint8_t* buffer, uint32_t size) :
    _arena{arena}, _buffer{buffer}, _size{size} {
    // DO NOTHING
}

uint8_t* Allocation::get() {
    return _buffer;
}

Allocation::~Allocation() {
    _arena.deallocate(_size);
}

ArenaAllocator::ArenaAllocator(uint32_t size) {
    _buffer = std::make_unique<uint8_t[]>(size);
    _size = size;
}

Allocation ArenaAllocator::allocate(uint32_t size) {
    if (_position + size > _size) {
        printf("pos=[%u] size_left=[%u] size_requested=[%u]\n",
               _position, _size - _position, size);
        throw std::runtime_error("doesn't have memory to allocate");
    }

    uint8_t* buffer = &_buffer.get()[_position];
    printf("PreAllocation pos=[%u]\n", _position);
    _position += size;
    printf("PostAllocation pos=[%u] size_left=[%u]\n", _position, _size - _position);
    ArenaAllocator& arena = *(this);
    return Allocation(arena, buffer, size);
}

void ArenaAllocator::deallocate(uint32_t size) {
    _position -= size;
    printf("post dealloc pos=[%u] size_left=[%u]\n", _position, _size - _position);
}
