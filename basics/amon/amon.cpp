#include <Arduino.h>

#ifndef PIN
  #define PIN A0
#endif

#ifndef DELAY
  #define DELAY 100
#endif

void setup () {
    Serial.begin (9600);
    pinMode (PIN, INPUT);
}

void loop () {
    auto x = analogRead (PIN);
    Serial.print (x, HEX);
    Serial.print (":");
    delay (DELAY);
}

