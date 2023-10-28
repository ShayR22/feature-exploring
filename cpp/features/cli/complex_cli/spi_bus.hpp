
#pragma once

#include <cstdint>

class SpiBus {
public:
    SpiBus(uint8_t bus) : _bus{bus}{
        // DO NOTHING
    }

private:
    uint8_t _bus;
};