// arduino.h - Minimal Arduino compatibility for Raspberry Pi (C++17)
#pragma once
#include <stdint.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <time.h>
#include <cmath>

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.57079632679489661923
#define TWO_PI  6.28318530717958647693
#define DEG_TO_RAD 0.01745329251994329577
#define RAD_TO_DEG 57.2957795130823208768

#define HIGH 1
#define LOW  0
#define INPUT 0
#define OUTPUT 1

#define MSBFIRST 1
#define LSBFIRST 0

inline void delay(unsigned long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

inline void delayMicroseconds(unsigned int us) {
    std::this_thread::sleep_for(std::chrono::microseconds(us));
}

inline unsigned long millis() {
    using namespace std::chrono;
    static auto start = steady_clock::now();
    return duration_cast<milliseconds>(steady_clock::now() - start).count();
}

inline unsigned long micros() {
    using namespace std::chrono;
    static auto start = steady_clock::now();
    return duration_cast<microseconds>(steady_clock::now() - start).count();
}

inline void pinMode(int, int) {}
inline void digitalWrite(int, int) {}
inline int digitalRead(int) { return 0; }
