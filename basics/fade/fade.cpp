#include <Arduino.h>
#ifdef ARDUINO_ARCH_ESP32
    #include <analogWrite.h>
#endif

uint8_t PINS[] {9, 10, 6};
#ifndef DELAY_HIGH
    #define DELAY_HIGH 4
#endif
#ifndef DELAY_LOW
    #define DELAY_LOW 4
#endif

void setup () {
    for (auto p : PINS)
        pinMode (p, OUTPUT);
}

void loop () {
    for (auto c = 0; c <= 255; c ++) {
        for (auto p : PINS)
            analogWrite (p, c);
        delay (DELAY_HIGH);
    }
    for (auto c = 255; c >= 0; c --) {
        for (auto p : PINS)
            analogWrite (p, c);
        delay (DELAY_LOW);
    }
}

