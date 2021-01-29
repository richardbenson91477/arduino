// Digispark Nunchuck shield demo
#include <Nunchuk_idm.h>
#include <Wire.h>

Nunchuk nck = Nunchuk ();

void setup () {
    Serial.begin (9600);
    Nunchuk::init ();
}

void loop () {
    nck.update ();

    Serial.println (nck.ax, DEC);
    Serial.println (nck.ay, DEC);
    Serial.println (nck.rx, DEC);
    Serial.println (nck.ry, DEC);
    Serial.println (nck.rz, DEC);
    Serial.println (nck.z, DEC);
    Serial.println (nck.c, DEC);

    delay (250);
}

