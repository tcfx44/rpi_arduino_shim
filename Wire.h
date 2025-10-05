// Wire.h - Minimal TwoWire (I2C) implementation for Raspberry Pi
#pragma once

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string>

class TwoWire {
    int fd = -1;
    uint8_t lastAddr = 0;
    std::vector<uint8_t> txBuffer;
    std::vector<uint8_t> rxBuffer;
    size_t rxIndex = 0;
public:
    // Open the i2c device (default /dev/i2c-1)
    bool begin(int sda_pin = -1, int scl_pin = -1) {
        const char *device = "/dev/i2c-1";
        fd = open(device, O_RDWR);
        return fd >= 0;
    }

    void end() {
        if (fd >= 0) { close(fd); fd = -1; }
    }

    void beginTransmission(uint8_t address) {
        lastAddr = address;
        txBuffer.clear();
    }

    size_t write(uint8_t data) {
        txBuffer.push_back(data);
        return 1;
    }

    size_t write(const uint8_t *data, size_t len) {
        txBuffer.insert(txBuffer.end(), data, data + len);
        return len;
    }

    // endTransmission returns 0 on success (Arduino-style)
    uint8_t endTransmission(bool sendStop = true) {
        (void)sendStop; // Linux i2c write always ends with stop condition
        if (fd < 0) return 4; // other error
        if (ioctl(fd, I2C_SLAVE, lastAddr) < 0) return 4;
        ssize_t r = ::write(fd, txBuffer.data(), txBuffer.size());
        return (r == (ssize_t)txBuffer.size()) ? 0 : 4;
    }

    // requestFrom: reads `len` bytes into internal buffer, returns number of bytes read
    int requestFrom(uint8_t address, uint8_t len) {
        if (fd < 0) return 0;
        if (ioctl(fd, I2C_SLAVE, address) < 0) return 0;
        rxBuffer.resize(len);
        ssize_t r = ::read(fd, rxBuffer.data(), len);
        if (r < 0) return 0;
        rxIndex = 0;
        return (int)r;
    }

    int available() { return (int)(rxBuffer.size() - rxIndex); }

    int read() {
        if (rxIndex < rxBuffer.size()) return rxBuffer[rxIndex++];
        return -1;
    }

    // Convenience: write then read (some devices require register write then read)
    // This method writes the register (single byte) then reads len bytes.
    int writeRead(uint8_t address, const uint8_t *tx, size_t txlen, uint8_t *rx, size_t rxlen) {
        if (fd < 0) return 0;
        if (ioctl(fd, I2C_SLAVE, address) < 0) return 0;
        // write register/command
        if (::write(fd, tx, txlen) != (ssize_t)txlen) return 0;
        // read reply
        ssize_t r = ::read(fd, rx, rxlen);
        if (r < 0) return 0;
        return (int)r;
    }

    void setClock(uint32_t clock) {
        // ignore, Linux i2c-dev uses system I2C clock
    }
};

inline TwoWire Wire;
