#include <Arduino.h>
#include <Nunchuk_idm.h>
#include <OLED_idm.h>

Nunchuk nck;
OLED oled;

void setup () {
    nck.init ();
    oled.begin ();
    oled.clear ();
}

void loop () {
    nck.update ();
    uint8_t nx = nck.ax >> 1;

    oled.rect (nx, 7, nx + 1, 8, 0xff);
    delay (10);
    oled.rect (nx, 7, nx + 1, 8, 0x0);
}

