
#pragma once

#include <cstdint>
#include <cstdio>

class SpiBus {
public:
    SpiBus(uint8_t bus) : _bus{bus}{
        printf("Ctor bus=%u\n", _bus);
        // DO NOTHING
    }

private:
    uint8_t _bus;
};