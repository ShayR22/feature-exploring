#include "cli11/CLI11.hpp"
#include <random>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cstdio>

static uint8_t rand_in_range(uint8_t start, uint8_t stop) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(start, stop); // define the range

    return distr(gen);
}


class SpiAccess {
public:
    SpiAccess(uint8_t bus_number) : _bus_number{bus_number} {
        // DO NOTHING
    }

    int write_reg(const uint32_t address, const uint32_t value) {
        printf("%s address=0x%08x, value=0x%08x\n", __func__, address, value);
        return 0;
    }

    int read_reg(const uint32_t  address, uint32_t& value) {
        value = rand_in_range(0, 255);
        printf("%s address=0x%08x, value=0x%08x\n", __func__, address, value);
        return 0;
    }
private:
    uint8_t _bus_number;
};

int main(int argc, char** argv) {
    CLI::App app{"SpiAccess CLI"};

    uint8_t bus_number = 0;
    app.add_option("-b,--bus_number", bus_number, "SPI bus number")->required();

    CLI::App* write_cmd = app.add_subcommand("write", "Write to SPI register");
    uint32_t write_address = 0;
    uint32_t write_value = 0;
    write_cmd->add_option("address", write_address, "Register address")->required();
    write_cmd->add_option("value", write_value, "Register value")->required();
    write_cmd->callback([&bus_number, &write_address, &write_value]() {
        SpiAccess spi{bus_number};
        int result = spi.write_reg(write_address, write_value);
        if (result != 0) {
            std::cerr << "Error writing to register\n";
            exit(1);
        }
    });

    CLI::App* read_cmd = app.add_subcommand("read", "Read from SPI register");
    uint32_t read_address = 0;
    read_cmd->add_option("address", read_address, "Register address")->required();
    read_cmd->callback([&bus_number, &read_address]() {
        SpiAccess spi{bus_number};
        uint32_t read_value = 0;
        int result = spi.read_reg(read_address, read_value);
        if (result != 0) {
            std::cerr << "Error reading from register\n";
            exit(1);
        }
        std::cout << "Register value: " << read_value << std::endl;
    });

    // run the CLI
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return app.exit(e);
    }

    return 0;
}