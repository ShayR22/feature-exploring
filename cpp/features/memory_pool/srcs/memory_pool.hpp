#pragma once

#include <vector>
#include <iostream>
#include <stack>
#include <memory>
#include <functional>
#include <cstdio>


template<typename T>
using PoolUniquePtr = std::unique_ptr<T, std::function<void(T*)>>;


class MemoryPool {
public:
    MemoryPool(uint32_t block_size, uint32_t num_blocks) :
        pool_{std::make_unique<uint8_t[]>(block_size * num_blocks)},
        block_size_{block_size} {
        Block* p = reinterpret_cast<Block*>(pool_.get());
        for (uint32_t i = 0; i < num_blocks; i++) {
            freeList.push(p);
            p = reinterpret_cast<Block*>(reinterpret_cast<uint8_t*>(p) + block_size);
            printf("p=[%p]\n", p);
        }
    }

    template <typename T, typename... Args>
    PoolUniquePtr<T> allocate(Args&&... args) {
        if (freeList.empty()) {
            // maybe change to return nullptr;
            throw std::bad_alloc();
        } else if (sizeof(T) > block_size_) {
            throw std::bad_alloc();
        }

        Block* head = freeList.top();
        freeList.pop();

        T* alloc_location_ptr = reinterpret_cast<T*>(head);
        T* obj = std::construct_at(alloc_location_ptr, std::forward<Args>(args)...);

        auto deleter = [this](T* ptr) -> void {
            deallocate<T>(ptr);
        };

        PoolUniquePtr<T> ptr(obj, deleter);
        return ptr;
    }

private:
    struct Block {
        Block* next;
    };

    std::stack<Block*> freeList;
    std::unique_ptr<uint8_t[]> pool_;
    uint32_t block_size_;

    template <typename T>
    void deallocate(T* pointer) {
        pointer->~T();
        auto block = reinterpret_cast<Block*>(pointer);
        freeList.push(block);
    }
};
