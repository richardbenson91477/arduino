#include <Arduino.h>

const uint8_t mic_pin = A3;
// attiny85 const uint8_t led_pins [] = {0, 1, 4};
// nano const uint8_t led_pins [] = {3, 6, 9};
const uint8_t led_pins [] = {3, 6, 9};
const uint8_t sleep_len = 1;

void setup () {
//Serial.begin (9600);
    pinMode (mic_pin, INPUT);
    for (auto c : led_pins) {
        pinMode (c, OUTPUT);
        analogWrite (c, 0);
    }
 }

void loop () {
    auto i = analogRead (mic_pin);
//Serial.print (i,HEX); Serial.write (':');
    if (i < 128)
        analogWrite (led_pins[0], 0);
    else 
        analogWrite (led_pins[0], i / 4);
    if (i < 256)
        analogWrite (led_pins[1], 0);
    else
        analogWrite (led_pins[1], i / 4);
    if (i < 512)
        analogWrite (led_pins[2], 0);
    else
        analogWrite (led_pins[2], i / 4);

    delay (sleep_len);
}

