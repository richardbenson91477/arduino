#include <Arduino.h>
// designed for MEGA 2560

#define EE_28C16A15
// #define EE_28C25615

#define W_DELAY 8
#define W_TRIES 8

#ifdef EE_28C16A15
    #define NAB 11
    const uint8_t PIN_A[NAB] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42};
#elif EE_28C25615
    #define NAB 15
    const uint8_t PIN_A[NAB] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, \
                                44, 46, 48, 50};
#endif

#define NDB 8
const uint8_t PIN_D[NDB] = {14, 15, 16, 17, 18, 19, 20, 21};
const uint8_t PIN_W = 52;
const uint8_t PIN_R = 53;

uint32_t BITS[NAB];
uint32_t address;

void setup () {
    address = 0;

    pinMode (PIN_W, OUTPUT);
    digitalWrite (PIN_W, HIGH);

    pinMode (PIN_R, OUTPUT);
    digitalWrite (PIN_R, LOW);
    
    BITS[0] = 1;
    for (uint8_t c = 1; c < NAB; c ++) {
        BITS[c] = BITS[c - 1] * 2;
    }

    for (uint8_t c = 0; c < NAB; c ++) {
        pinMode (PIN_A[c], OUTPUT);
        digitalWrite (PIN_A[c], LOW);
    }

    for (uint8_t c = 0; c < NDB; c ++) {
        pinMode (PIN_D[c], INPUT);
    }

    Serial.begin (9600);
    Serial.print ("\r\n?");
}

void loop () {
    while (! Serial.available())
        delay (10);

    uint8_t b = Serial.read();
    switch (b) {
        case 'w':
            while (! Serial.available())
                delay (1);
            b = Serial.read();
            eep_write (address, b);
            Serial.print ("?");
            break;

        case 'a':
            address = 0;
            for (uint8_t c = 0; c < (NAB / 8) + ((NAB % 8) ? 1 : 0); c ++) {
                while (! Serial.available())
                    delay (1);
                b = Serial.read();
                address |= ((uint32_t)b << (c * 8));
            }
            Serial.print ("?");
            break;

        case 'n':
            address ++;
            Serial.print ("?");
            break;

        case 'p':
            address --;
            Serial.print ("?");
            break;

        case 'r':
            b = eep_read (address);
            Serial.print (address);
            Serial.print (":");
            Serial.print (b);
            Serial.print ("\r\n?");
            break;

        case 'b':
            b = eep_read (address);
            Serial.write (&b, 1);
            Serial.print ("?");
            break;
    }
}

uint8_t eep_read (uint32_t a) {
    for (uint8_t c = 0; c < NAB; c ++) {
        digitalWrite (PIN_A[c], a & BITS[c] ? HIGH : LOW);
    }

    uint8_t d = 0;
    for (uint8_t c = 0; c < NDB; c ++) {
        if (digitalRead (PIN_D[c]))
           d |= BITS[c];
    }

    return d;
}

void eep_write (uint32_t a, uint8_t d) {
    for (uint8_t t = 0; t < W_TRIES; t ++) {
        for (uint8_t c = 0; c < NAB; c ++) {
            digitalWrite (PIN_A[c], (a & BITS[c]) ? HIGH : LOW);
        }
 
        for (uint8_t c = 0; c < NDB; c ++) {
            pinMode (PIN_D[c], OUTPUT);
        }

        digitalWrite (PIN_R, HIGH);
        digitalWrite (PIN_W, LOW);

        for (uint8_t c = 0; c < NDB; c ++) {
            digitalWrite (PIN_D[c], (d & BITS[c]) ? HIGH : LOW);
        }
    
        digitalWrite (PIN_W, HIGH);
        digitalWrite (PIN_R, LOW);
    
        delay (W_DELAY);
    
        for (uint8_t c = 0; c < NDB; c ++) {
            pinMode (PIN_D[c], INPUT);
        }

        uint8_t b = eep_read (a);
        if (b == d)
            return;

        delay (W_DELAY);

        b = eep_read (a);
        if (b == d)
            return;

        Serial.print(b);
        Serial.print(" is not ");
        Serial.print(d);
    }

    // fail spectacularly
    Serial.print ("FAILURE\r\n?");
    while (1);
}

