#include <Arduino.h>
//#include <avr/pgmspace.h>

#ifndef APIN
  #define APIN 0
#endif

void dump256 () {
    uint16_t a[256];

    for (auto cl = 0; cl < 256; cl ++)
        a[cl] = analogRead (APIN);

    Serial.write ((uint8_t *) a, 512);  
}

void dumpn (uint16_t n) {
    for (auto cl = 0; cl < n; cl ++) {
        auto a = analogRead (APIN);
        Serial.write ((uint8_t *) &a, 2);
    }
}

uint16_t getn () {
    uint16_t n;

    Serial.readBytes ((char *) &n, 2);
    return n;
}

void prompt () {
    Serial.print ("\r\n?");
}

void setup () {
    Serial.begin (9600);
    prompt();
}

void loop () {
    auto sn = Serial.available ();
    while (sn --) {
        auto sc = Serial.read ();
        switch (sc) {
            case 'r':
                dump256 ();
                prompt ();
                break;
            
            case 'n':
                dumpn (getn ());
                prompt ();
                break;

            default:
                prompt ();
        }
    }
}

