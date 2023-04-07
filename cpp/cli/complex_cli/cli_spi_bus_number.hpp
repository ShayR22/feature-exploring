
#pragma once

#include "../cli11/CLI11.hpp"
#include <cstdint>


class CliSpiBusNumber {
public:
    CliSpiBusNumber(CLI::App& app, uint8_t default_value) {
        // set default value
        _bus_number = default_value;
        std::string description_str = "SPI bus number, default " + std::to_string(default_value);

        app.add_option("-b,--bus_number",
                        _bus_number,
                        description_str);
    }

    uint8_t get_bus_number() {
        return _bus_number;
    }

private:
    uint8_t _bus_number;
};
