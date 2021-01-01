#include <Arduino.h>
//#include <avr/pgmspace.h>

const uint8_t PINS [] = {2, 3, 4, 5, 6, 7, 8, 9};
const uint8_t BITS [] = {1, 2, 4, 8, 16, 32, 64, 128};
#ifndef DELAY
    #define DELAY 800
#endif

void setup () {
    Serial.begin (9600);
    Serial.print ("\r\n");

    for (uint8_t cl = 0; cl < 8; cl ++)
        pinMode (PINS[cl], INPUT);
}

void loop () {  
    uint8_t d = 0;
    for (uint8_t cl = 0; cl < 8; cl ++)
        if (digitalRead (PINS[cl]))
            d |= BITS[cl];

    Serial.print (d);
    Serial.print (" ");

    delay (DELAY);
}

