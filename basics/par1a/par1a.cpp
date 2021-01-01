#include <Arduino.h>

#ifndef PINCLK
    #define PINCLK 2
    #define PINDI 3
    #define PINDO 4
#endif

#define IBUFN 64

const uint8_t BITS8[8] = {1, 2, 4, 8, 16, 32, 64, 128};
const uint8_t BITS8N[8] = {254, 253, 251, 247, 239, 223, 191, 127};

volatile uint8_t bufi [IBUFN];
volatile uint8_t bufc, bitc, clk8;

void on_clk (void) {
    if (digitalRead (PINDI) == HIGH)
        bufi [bufc] |= BITS8 [bitc];
    else
        bufi [bufc] &= BITS8N [bitc];

    bitc ++;
    if (bitc > 7) {
        bitc = 0;

        bufc ++;    
        if (bufc > IBUFN) {
            bufc = 0;
            clk8 = 0;
        }
        else {
            clk8 = 1;
        }
    }
}

void initbuf () {
    bufc = bitc = clk8 = 0;
}

void setup () {
    pinMode (PINDO, OUTPUT);

    Serial.begin (9600);

    initbuf ();

    attachInterrupt (PINCLK -2, on_clk, RISING);
/*
    Serial.print ("sending request for 100ms.\r\n");
    digitalWrite (PINDO, HIGH);
    delay (100);
    digitalWrite (PINDO, LOW);
*/
    Serial.print ("\r\nrelaying.\r\n");
}

void loop () {
    uint8_t nc;

    if (! clk8) {
        delay (100);
        if (! clk8) return;
    }
    clk8 = 0;
    
    Serial.write ((const uint8_t *) bufi, bufc);
    /*for (nc =0; nc < bufc; nc ++) {
        Serial.print (bufi [nc]);
        Serial.print ("\r\n");
    }*/

    if (bitc) 
        bufi [0] = bufi [bufc];
    bufc = 0;
}

