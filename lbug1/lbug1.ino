#include <Arduino.h>
#include <Servo.h>

#include "seq.h"

Servo srv;

#define BUFSZ 16

uint8_t OM[] = {8, 9};
uint8_t OL[] = {4, 5};
uint8_t OP = 6;
uint8_t OS = 7;
uint8_t IA0 = 0;
uint8_t IPB = 3;

volatile uint8_t pb = 0;

uint16_t seq_n; // n sequences
uint16_t seq_cn; // current sequence
uint16_t seq_cs; // current sequence size
uint8_t *seq_cp; // pointer to current sequence
uint16_t seq_co; // offset into current sequence

void pushed () {
    pb = 1;
}

void initseq () {
    seq_n = *(uint16_t *)(seq);
    seq_cn = 0;
    seq_cs = *(uint16_t *)(seq +2);
    seq_cp = (uint8_t *)seq +2 +(seq_n *2);
    seq_co = 0;
}

void stop () {
    digitalWrite (OM[0], LOW); digitalWrite (OM[1], LOW);
    digitalWrite (OL[0], LOW); digitalWrite (OL[1], LOW);
    noTone (OP);
}

void setup () {
    pinMode (OM[0], OUTPUT); pinMode (OM[1], OUTPUT);
    pinMode (OL[0], OUTPUT); pinMode (OL[1], OUTPUT);
    pinMode (OP, OUTPUT);
    pinMode (OS, OUTPUT);
    pinMode (IPB, INPUT);
    
    digitalWrite (OM[0], LOW); digitalWrite (OM[1], LOW);
    digitalWrite (OL[0], LOW); digitalWrite (OL[1], LOW);
    digitalWrite (OP, LOW);

    initseq ();

    srv.attach (OS);
    srv.write (90);

    attachInterrupt (IPB -2, pushed, FALLING);

    Serial.begin (9600);
    Serial.print ("?");
    Serial.flush ();
}

void flerp (uint16_t f, uint16_t t, uint16_t s, uint16_t d, uint16_t n) {
    uint16_t q;
    for (uint16_t i = 0; i < n; i ++) { 
        if (t > f) {
            for (q = f; q <= t; q += s) {
                tone (OP, q);
                delay (d);
            }
        }
        else {
            for (q = f; q >= t; q -= s) {
                tone (OP, q);
                delay (d);
            }
        }
    }

    noTone (OP);
}

uint8_t cmd (const uint8_t *s) {
    switch (s[0]) {
        case 'f': // piezo freq interpolation 
            flerp (*(uint16_t *)(s +1), *(uint16_t *)(s +3),
                *(uint16_t *)(s +5), *(uint16_t *)(s +7),
                *(uint16_t *)(s +9));
            return 11;

        case 'p': // piezo freq
            tone (OP, *(uint16_t *)(s +1));
            return 3;

        case 'P': // piezo stop
            noTone (OP);
            return 1;

        case 's': // servo
            srv.write (s[1]);
            return 2;

        case 'l': // LEDs
            if (s[1]) digitalWrite (OL[0], HIGH);
            else digitalWrite (OL[0], LOW);

            if (s[2]) digitalWrite (OL[1], HIGH);
            else digitalWrite (OL[1], LOW);
            
            return 3;

        case 'm': // motors
            if (s[1]) digitalWrite (OM[0], HIGH);
            else digitalWrite (OM[0], LOW);

            if (s[2]) digitalWrite (OM[1], HIGH);
            else digitalWrite (OM[1], LOW);
            return 3;

        case 'd': // delay
            delay (*(uint16_t *)(s +1));
            return 3;   
    };
}

void sread (uint16_t n) {
    uint8_t inb[BUFSZ];
    uint8_t i;

    for (i = 0; i < n; i ++)
        inb[i] = Serial.read();

    cmd (inb);
    Serial.print ("*");
    Serial.flush ();
}

void loop () {
    uint16_t a;

    if (a = Serial.available ()) {
        sread (a);
    }
    else if (pb) {
        pb = 0;
        
        seq_cn ++;
        if (seq_cn >= seq_n) {
            initseq ();
            stop ();
            return;
        }

        seq_cp += seq_cs;
        seq_cs = *(uint16_t *)(seq +2 +(seq_cn *2));
        seq_co = 0;

        stop ();
    }
    else {
        seq_co += cmd (seq_cp +seq_co);
        if (seq_co >= seq_cs) seq_co = 0;
    }

}

