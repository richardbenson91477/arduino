#include <Arduino.h>
//#include <avr/pgmspace.h>
#define DEBUG 1

#define BUF_N 80
char in_b[BUF_N];
char out_b[BUF_N];
uint8_t in_c = 0;

#define NL 8
const uint8_t PINS[] = { 8, 9, 10, 5, 4, 7, 6, 11 };
const uint8_t BITS[] = { 1, 2, 4, 8, 16, 32, 64, 128 };

#define NC 36
const uint8_t CODE[NC] = {
    // DIGITS
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 
    0x7f, 0x6f,
    // A-F
    0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71,
    // G-Z
    0x3d, 0x76, 0x30, 0xe, 0x72, 0x38, 0x55, 0x54,
    0x5c, 0x73, 0x67, 0x50, 0x6c, 0x31, 0x3e, 0x1c,
    0x49, 0x52, 0x6a, 0x1b
};

uint16_t DELAY1 = 500;
uint16_t DELAY2 = 100;
uint16_t DELAY3 = 200;

volatile uint8_t but = 0;

void ss_print_binary (const char *s) {
    for (auto m = (char *)s; *m; m ++) {
        for (auto cl = 0; cl < NL; cl ++) {
            digitalWrite (PINS[cl], *m & BITS[cl]);
        }
        delay (DELAY1);
        for (auto cl = 0; cl < NL; cl++)
            digitalWrite (PINS[cl], LOW);      
        delay (DELAY2);
    }
    delay (DELAY3);
}

void ss_print_string (const char *s) {
    uint8_t c;
    for (auto m = (char *)s; *m; m ++) {
        if ((*m >= '0') && (*m <= '9'))
            c = *m - '0';
        else if ((*m >= 'a') && (*m <= 'z'))
            c = *m - 'a' + 10;
        else if ((*m >= 'A') && (*m <= 'Z'))
            c = *m - 'A' + 10;
        else
            continue;

        for (auto cl = 0; cl < NL; cl ++)
            digitalWrite (PINS[cl], (CODE[c] & BITS[cl]));

        delay (DELAY1);

        for (auto cl = 0; cl < NL; cl++) 
            digitalWrite (PINS[cl], LOW);

        delay (DELAY2);
    }

    Serial.println (s);
    delay (DELAY3);
}

void ss_print_chars (void) {
    for (auto w = 0; w < NC; w ++) {
        for (auto cl = 0; cl < NL; cl ++)
            digitalWrite (PINS[cl], (CODE[w] & BITS[cl]));
 
        delay (10);
    }

    for (auto cl = 0; cl < NL; cl++) 
        digitalWrite (PINS[cl], LOW);

    delay (DELAY3);
}

void interp (const char *s) {
    uint16_t i16a, i16b;
    uint8_t l;
    char *m, *m2;

    l = strlen (s);
    if (l < 2)
        goto ERR;

    switch (*s) {
        case 'P':
        case 'p':
            m = strchr (s, ' ');
            if (! m)
                goto ERR;
            m ++;
            if (! *m)
                goto ERR;

            ss_print_string (m);
            return;
            break;

        case 'A':
        case 'a':
            m = strchr (s, ' ');
            if (! m)
                goto ERR;
            m ++;
            if (! *m)
                goto ERR;

            m2 = strchr (m, ' ');
            if (! m2)
                goto ERR;
            m2 ++;
            if (! *m2)
                goto ERR;

            i16a = atoi (m);
            i16b = atoi (m2);
            sprintf (out_b, "%d", i16a + i16b);

            ss_print_string (out_b);
            return;
            break;
    }

ERR:
    ss_print_string ("ERR");
}

void on_button () {
    but = 1;
}

void setup () {
    Serial.begin (9600);

    for (auto cl = 0; cl < NL; cl ++) {
        pinMode (PINS[cl], OUTPUT);
        digitalWrite (PINS[cl], LOW);
    }

    pinMode (2, INPUT);
    digitalWrite (2, LOW);

    attachInterrupt (0, on_button, RISING);

    //randomSeed (analogRead (0));

    ss_print_chars ();
    ss_print_string ("RD");
}

void loop () {
    auto a = Serial.available ();
    while (a--) {
        auto r = Serial.read ();

        if ((r == '\r') || (in_c == (BUF_N -1))) {
            in_b [in_c] = 0;
            in_c = 0;
         
            Serial.write ("\r\n");
            interp (in_b);
            ss_print_string ("RD");
            return;
        }
        else if (r == '\n') {
            return;
        }

        in_b [in_c ++] = r;
        Serial.write (r);
    }

    if (but) {
        but = 0;
        ss_print_string ("HI");
    }
}

