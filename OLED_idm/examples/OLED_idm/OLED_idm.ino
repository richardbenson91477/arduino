#include <Arduino.h>
#include <OLED_idm.h>
#include "bmp.h"
#include "font6x8.h"

// pin to read for random value
#define RANDOM_ANALOG_PIN A3

OLED oled;

void setup () {
//    Serial.begin (9600);

    oled.begin ();
    oled.setfont (font6x8_w, font6x8);

    randomSeed (analogRead (RANDOM_ANALOG_PIN));
}

void loop () {
    // fill and clear
    for (uint8_t c = 0; c < 4; c ++) {
        oled.fill (0xff);
        delay (250 * (3 - c));
        oled.clear ();
        delay (250 * (3 - c));
    }
    
    // bitmap
    oled.fill (0xff);
    oled.bitmap (0, 0, 128, 8, bmp);
    delay (2000);
    oled.clear ();

    // print
    oled.clear ();
    for (uint8_t c = 0; c < 4; c ++) {
        for (uint8_t ch = 0; ch <= (sizeof(font6x8) / font6x8_w); ch ++) {
           oled.print (ch);
        }
        delay (1000);
    }

    // rect
    oled.clear ();
    uint8_t tmp;
    for (uint16_t c = 0; c < 32; c ++) {
        uint8_t x0 = random (oled.res_x);
        uint8_t y0 = random (oled.res_yp);
        uint8_t x1 = random (oled.res_x) + 1;
        uint8_t y1 = random (oled.res_yp) + 1;
        if (x0 > x1) { tmp = x0; x0 = x1; x1 = tmp; }
        if (y0 > y1) { tmp = y0; y0 = y1; y1 = tmp; }
        oled.rect (x0, y0, x1, y1, 0xff);
        // delay based on area
        delay (256 - (x1 - x0) * (y1 - y0) / 4);
        oled.rect (x0, y0, x1, y1, 0x0);
    }
    
    // pixel_on/off
    uint8_t px = oled.res_x / 2,
        py = oled.res_yp * 4;
    for (uint16_t c = 0; c < 1024; c ++) {
        px += 1 - random (3);
        if (px == oled.res_x)
            px = oled.res_x - 1;
        else if (px == 0)
            px = 1;
        py += 1 - random (3);
        if (py == oled.res_yp * 8)
            py = (oled.res_yp * 8) - 1;
        else if (py == 0)
            py = 1;
        oled.pixel_on (px, py);
        delay (4);
        oled.pixel_off (px, py);
    }
}

