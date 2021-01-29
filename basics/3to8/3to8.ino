//#include <avr/pgmspace.h>

#define OUT1 4

#ifndef DELAY
    #define DELAY 500
#endif

void setup () {
    uint8_t cl;
  
    for (cl = OUT1; cl < (OUT1 + 4); cl ++) {
        pinMode (cl, OUTPUT);
        digitalWrite (cl, LOW);
    }

    for (;;) {
        for (cl = 0; cl < 8; cl ++) {
            digitalWrite (OUT1, cl & 1);
            digitalWrite (OUT1 + 1, cl & 2);
            digitalWrite (OUT1 + 2, cl & 4);
            delay (DELAY);
        }
    }
}

void loop () {
}

