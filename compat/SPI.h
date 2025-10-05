// SPI.h - Minimal SPI implementation for Raspberry Pi
#pragma once

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

class SPISettings {
public:
  SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
    if (__builtin_constant_p(clock)) {
      init_AlwaysInline(clock, bitOrder, dataMode);
    } else {
      init_MightInline(clock, bitOrder, dataMode);
    }
  }
  SPISettings() {
    init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0);
  }

  uint32_t clock;
  uint8_t bitOrder;
  uint8_t dataMode;

private:
  void init_AlwaysInline(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) __attribute__((always_inline)) {
    this->clock = clock;
    this->bitOrder = bitOrder;
    this->dataMode = dataMode;
  }
  void init_MightInline(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
    this->clock = clock;
    this->bitOrder = bitOrder;
    this->dataMode = dataMode;
  }
};

class SPIClass {
    int fd = -1;
    uint8_t mode = SPI_MODE_0;
    uint32_t speed = 1000000; // 1 MHz default
    uint8_t bits = 8;
    const char *device = "/dev/spidev0.0";
public:
    bool begin(const char *dev = "/dev/spidev0.0") {
        device = dev;
        fd = open(device, O_RDWR);
        if (fd < 0) return false;
        if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) return false;
        if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) return false;
        if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) return false;
        return true;
    }
    void end() {
        if (fd >= 0) { close(fd); fd = -1; }
    }
    void beginTransaction(SPISettings settings) {
        mode = settings.dataMode;
        speed = settings.clock;
        if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) return;
        if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) return;
        if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) return;
    }
    
    void endTransaction() {}
    void setClockDivider(uint32_t hz) { speed = hz; }
    void setDataMode(uint8_t m) { mode = m; }
    uint8_t transfer(uint8_t tx) {
        uint8_t rx = 0;
        if (fd < 0) return 0;
        struct spi_ioc_transfer tr;
        memset(&tr, 0, sizeof(tr));
        tr.tx_buf = (unsigned long)&tx;
        tr.rx_buf = (unsigned long)&rx;
        tr.len = 1;
        tr.speed_hz = speed;
        tr.bits_per_word = bits;
        if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) return 0;
        return rx;
    }
    void transfer(void* txbuf, void* rxbuf, size_t len) {
        if (fd < 0) return;
        struct spi_ioc_transfer tr;
        memset(&tr, 0, sizeof(tr));
        tr.tx_buf = (unsigned long)txbuf;
        tr.rx_buf = (unsigned long)rxbuf;
        tr.len = len;
        tr.speed_hz = speed;
        tr.bits_per_word = bits;
        ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    }
};

inline SPIClass SPI;
