#include "font.h"
#include <Arduino.h>
#include <OLED_idm.h>
//#include <avr/power.h>

// detect digispark/tiny
#ifdef ATTINY_CORE
  #define PIN_PADDLE A3
  #define PIN_RANDOM A0
#else
  #define PIN_PADDLE A7
  #define PIN_RANDOM A6
#endif

OLED oled;
// screen width
uint8_t s_w;
// player height (lives), width (in blocks), width (in px), x pos, max x pos,
uint8_t p_h, p_w, p_pw, p_x, p_mx;

bool player_init () {
    p_h = 3;

    p_w = 3;
    p_pw = p_w * font_w;

    p_x = (uint8_t) -1;
    p_mx = s_w - p_pw - 1;
}

void setup () {
    // clock_prescale_set (clock_div_1);
    oled.begin ();
    oled.clear ();
    oled.setfont (font_w, _font);
    s_w = oled.res_x;

    analogReference (DEFAULT);
    randomSeed (analogRead (PIN_RANDOM));

    player_init ();
}

void loop () {
    // read paddle as 0-128
    uint8_t in_x = analogRead (PIN_PADDLE) >> 3;
    // clamp to max
    if (in_x > p_mx)
        in_x = p_mx;
    // moved
    if (p_x != in_x) {
        // erase curent paddle if valid
        if (p_x != (uint8_t) -1)
            oled.rect (p_x, 7, p_x + p_pw, 8, 0x0);
        // change pos
        p_x = in_x;
        // draw new paddle
        oled.cursor_set (p_x, 7);
        for (uint8_t c = 0; c < p_w; c ++)
            oled.print (FONT_PLAYER_H0 + p_h);
    }

    delay (20);
}
