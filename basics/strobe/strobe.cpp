#include <Arduino.h>

uint8_t PINS[] {9, 10, 11};
#ifndef DELAY_HIGH
    #define DELAY_HIGH 1000
#endif
#ifndef DELAY_LOW
    #define DELAY_LOW 1000
#endif

void setup () {
    for (auto p : PINS)
        pinMode (p, OUTPUT);
}

void loop () {
    for (auto p : PINS)
        digitalWrite (p, HIGH);
    delay (DELAY_HIGH);
    for (auto p : PINS)
        digitalWrite (p, LOW);
    delay (DELAY_LOW);
}

