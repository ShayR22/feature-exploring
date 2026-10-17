
#pragma once
#include "spi_comm.hpp"

static constexpr uint32_t VERSION_REG_ADDRESS = 0X12345678;

struct VersionRegister {
    uint8_t v_a;
    uint8_t v_b;
    uint8_t v_c;
    uint8_t v_d;
};

class MySpiDevice {
public:
    MySpiDevice(SpiComm& spi_comm) : _spi_comm{spi_comm} {
        // DO NOTHING
    }

    int read_version(uint32_t& version) {
        int err = _spi_comm.read_reg(VERSION_REG_ADDRESS, version);
        if (err) {
            printf("%s: failed to read version err=%d\n", __func__, err);
            return err;
        }
        printf("%s: version=0x%02x\n", __func__, version);
        return 0;
    }

    int write_version(const uint8_t v_a, const uint8_t v_b, const uint8_t v_c, const uint8_t v_d) {
        uint32_t version = v_a << 24 | v_b << 16 | v_c << 8 | v_d;
        int err = _spi_comm.write_reg(VERSION_REG_ADDRESS, version);
        if (err) {
            printf("%s: failed to write version err=%d\n", __func__, err);
            return err;
        }
        printf("%s: version=0x%08x\n", __func__, version);
        return 0;
    }
private:
    SpiComm _spi_comm;
};