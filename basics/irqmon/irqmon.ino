#include <Arduino.h>

#ifndef INTPIN
  #define INTPIN 2
#endif

#ifndef DELAY
  #define DELAY 1000
#endif

volatile uint16_t i_t = 0;

void on_int () {
    i_t += 1;
}

void setup () {
    attachInterrupt (INTPIN -2, on_int, CHANGE);

    Serial.begin (9600);
    Serial.print ("\r\n");
}

void loop () {
    delay (DELAY);
    if (i_t) {
        while (i_t) {
            Serial.print ("*");
            i_t --;
        }
    }
    else
        Serial.print ("_");
}

