#pragma once

#include "../cli11/CLI11.hpp"
#include "my_spi_device.hpp"
#include "cli_spi_bus_number.hpp"
#include "cli_runner.hpp"
#include <cstdint>
#include <cstdio>

class CliMySpiDev {
public:
    CliMySpiDev(uint8_t default_spi_bus_number) :
        _app{"MySpiDevice CLI"},
        _cli_bus_number(_app, default_spi_bus_number) {

        // overload the cli with commands
        _cli_read_version_command();
        _cli_write_version_command();
    }

    int run(int argc, char **argv) {
        return CliRunner::run(_app, argc, argv);
    }

private:
    CLI::App _app;
    CliSpiBusNumber _cli_bus_number;

    MySpiDevice& get_device() {
        // called in runtime as only in runtime required values
        // are present and being parsed by the cli11 wrapper
        // classes, thus, create static class and return reference
        // this is done for two reasons:
        // 1) only work with one actual device
        // 2) remove code duplication
        static SpiBus spi_bus{_cli_bus_number.get_bus_number()};
        static SpiComm spi_comm{spi_bus};
        static MySpiDevice my_spi_device{spi_comm};
        return my_spi_device;
    }

    void _cli_read_version_command() {
        CLI::App* read_version_cmd = _app.add_subcommand("read_version", "read version of the spi device");
        read_version_cmd->callback([&]() {
            MySpiDevice& my_spi_dev = get_device();
            uint32_t version{0};
            int err = my_spi_dev.read_version(version);
            if (err) {
                printf("%s: Failed to read version err=%d\n", __func__, err);
                return;
            }
            printf("%s: version=0x%02x\n", __func__, version);
        });
    }

    void _cli_write_version_command() {
        uint8_t v_a = 0;
        uint8_t v_b = 0;
        uint8_t v_c = 0;
        uint8_t v_d = 0;

        CLI::App* write_version_cmd = _app.add_subcommand("write_version", "write version of the spi device");
        write_version_cmd->add_option("-a, --va", v_a, "va value last 8 bits, default 0");
        write_version_cmd->add_option("-b, --vb", v_b, "vb value last 8 bits, default 0");
        write_version_cmd->add_option("-c, --vc", v_c, "vc value last 8 bits, default 0");
        write_version_cmd->add_option("-d, --vd", v_d, "vd value last 8 bits, default 0");

        write_version_cmd->callback([&]() {
            MySpiDevice& my_spi_dev = get_device();
            int err = my_spi_dev.write_version(v_a, v_b, v_c, v_d);
            if (err) {
                printf("%s: Failed to read version err=%d\n", __func__, err);
                return;
            }
            printf("%s: v_a=0x%02x v_b=0x%02x v_c=0x%02x v_d=0x%02x\n", __func__, v_a, v_b, v_c, v_d);

            uint32_t version = 0;
            err = my_spi_dev.read_version(version);
            if (err) {
                printf("%s: Failed to read version err=%d\n", __func__, err);
                return;
            }
            printf("%s: after version=0x%02x\n", __func__, version);

        });
    }

};
