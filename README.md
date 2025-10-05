\
        # Arduino shim for Raspberry Pi (I2C & SPI)

        This package provides a minimal Arduino-compatible shim (`arduino.h`, `Wire.h`, `SPI.h`) that routes I2C/SPI calls to Linux `/dev` devices so you can use the Arduino libraries(e.g. finani/ICM42688) with minimal changes on Raspberry Pi OS.

        ## Usage
        1. Ensure your user has access to `/dev/i2c-1` and `/dev/spidev0.0` (add to `i2c`/`spi` groups or run as root).
        2. Clone the finani repo next to this folder, or set `LIB_PATH` when building to point to your lib repo path.


        ### Build with CMake (recommended)
        ```bash
        cd examples
        mkdir build && cd build
        cmake -DLIB_PATH=/path/to/lib ..
        make
        ./lib_test
        ```

        ### Build with Makefile
        ```bash
        cd examples
        make LIB_PATH=/path/to/lib
        ./lib_test
        ```

        ## Notes
        - Default I2C device is `/dev/i2c-1`. Default SPI device is `/dev/spidev0.0`.
        - Adjust I2C address in `examples/lib_test.cpp` if your board uses a different address.

