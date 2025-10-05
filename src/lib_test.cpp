// Example program showing usage of the finani ICM42688 Arduino library on Raspberry Pi
// Build with -I/path/to/ICM42688/src
#include <iostream>
#include "Arduino.h"
#include "Wire.h"
#include "SPI.h"
#include "ICM42688.h" // from finani repo - ensure include path points to that repo's src folder

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    std::cout << "Starting ICM42688 test (Arduino shim on Raspberry Pi)\n";

    // Try to initialize Wire (I2C)
    if (!Wire.begin()) {
        std::cerr << "Warning: Wire.begin() failed (is /dev/i2c-1 available?).\n";
    } else {
        std::cout << "Wire (I2C) opened.\n";
    }

    // Also initialize SPI (optional, harmless if not used)
    if (!SPI.begin("/dev/spidev0.0")) {
        std::cerr << "Warning: SPI.begin() failed (is /dev/spidev0.0 available?).\n";
    } else {
        std::cout << "SPI opened.\n";
    }

    delay(100);

    // Example: construct the ICM42688 object
    // The finani library may provide constructors for Wire or for SPI.
    // This example uses a constructor with TwoWire &wire (reference, not pointer)
    uint8_t i2caddr = 0x68; // common I2C address; adjust if AD0/SA0 is different
    ICM42688 imu(Wire, i2caddr); // Pass Wire by reference, not pointer

    int result = imu.begin();

    if (result == 1) {
        std::cout << "ICM42688 initialized via Arduino shim!\n";
                
    } else {
        std::cerr << "ICM42688 initialization failed with error " << result << ".\n";
    }

    while(1) {
        int imu_status = imu.getAGT();

        float acc_x = imu.accX();
        float acc_y = imu.accY();
        float acc_z = imu.accZ();

        std::cout << "Status: " << imu_status 
                  << " Accel (m/s^2): X=" << acc_x 
                  << " Y=" << acc_y 
                  << " Z=" << acc_z << "\n";

        delay(50);

    }


    // cleanup
    Wire.end();
    SPI.end();
    return 0;
}
