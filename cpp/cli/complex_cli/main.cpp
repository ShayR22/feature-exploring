#include "../cli11/CLI11.hpp"
#include "cli_my_spi_dev.hpp"

static constexpr uint8_t DEFAULT_SPI_BUS_NUMBER = 1;

int main(int argc, char** argv) {
    CliMySpiDev cli{DEFAULT_SPI_BUS_NUMBER};
    int err = cli.run(argc, argv);
    if (err) {
        printf("%s: Failed to run CliMySpiDev\n", __func__);
    }

    return err;
}