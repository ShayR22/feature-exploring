
#pragma once

#include "spi_bus.hpp"
#include <cstdio>
#include <random>

class SpiComm {
public:
    SpiComm(SpiBus& spi_bus): _spi_bus{spi_bus} {
        // DO NOTHING
    }

    int write_reg(const uint32_t address, const uint32_t value) {
        printf("%s: address=0x%08x value=0x%08x\n", __func__, address, value);
        // do something with spi bus
        return 0;
    }

    int read_reg(const uint32_t address, uint32_t& value) {
        // do something with spi bus
        value = _rand_in_range(0, 255);
        printf("%s: address=0x%08x value=0x%08x\n", __func__, address, value);
        return 0;
    }
private:
    SpiBus _spi_bus;

    uint8_t _rand_in_range(uint8_t start, uint8_t stop) {
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(start, stop); // define the range
        return distr(gen);
    }

};
