#include <Arduino.h>

#ifndef PIN
    #define PIN 4
#endif

#ifndef DELAY
    #define DELAY 800
#endif

void setup () {
    pinMode (PIN, INPUT);

    Serial.begin (9600);
    Serial.print ("\r\n");
}

void loop () {
    if (digitalRead (PIN))
        Serial.print ("*");
    else
        Serial.print ("_");

    delay (DELAY);
}

