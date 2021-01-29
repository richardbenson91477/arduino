#include <Arduino.h>

#define SONG_MAXLEN 8

const uint8_t pins_led [] = {9, 6, 5, 3};
const uint8_t pins_joy [] = {0, 1};
const uint8_t pin_spk = 11;
const uint16_t tones [] = {110, 165, 220, 275};

int song [SONG_MAXLEN];
int input [SONG_MAXLEN];
int song_len, input_c;
int pos, pos_last;

void reset ();
void intro ();
void addnote ();
int verify ();
void play (int n);
void win ();
int read ();

void setup () {
    pinMode (pin_spk, OUTPUT);
    for (int c = 0; c < sizeof(pins_led); c++) {
        pinMode (pins_led[c], OUTPUT);
        digitalWrite (pins_led[c], LOW);
    }
 
    reset();
}

void loop () {
    if (read() && (pos != 4)) {
        play(pos);
        
        input[input_c++] = pos;
        
        if (! verify()) {
            reset();
            return;
        }
        
        if (input_c == song_len)
            win();
    }
    
    delay (50);
}

void reset () {
    song_len = 0;

    pos = 4; 
    pos_last = 4;

    intro();
    addnote();
}

void intro () { 
    for (int c = 0; c < 4; c++)
        digitalWrite (pins_led[c], HIGH);

    delay (200);

    for (int c = 0; c < 4; c++)
        digitalWrite (pins_led[c], LOW);
    
    delay (200);
}

void addnote () {
    int n;
    
    input_c = 0;

    song[song_len++] = random (4);
    for (int c = 0; c < song_len; c ++)
        play (song[c]);
}

int verify () {
    for (int c = 0; c < input_c; c ++)
        if (input[c] != song[c]) 
            return false;
    
    return true;
}

void play (int n) {
    digitalWrite (pins_led[n], HIGH);

    tone (pin_spk, tones[n], 290);
    delay (300);
    digitalWrite (pins_led[n], LOW);
    delay (10);
}

void win () {
    if ((song_len +1) == SONG_MAXLEN) {
        for (int c = 0; c < 4; c++)
            intro();
        reset();
        return;
    }

    delay (400);
    addnote();
}

int read () {
    float xf, yf;
    int x, y;
    static int xs = -1, ys;
    static int xmin, xmax, ymin, ymax;

    if (xs < 0) {
        xs = analogRead (pins_joy[0]);
        ys = analogRead (pins_joy[1]);
        xmin = xs - 200; ymin = ys - 200;
        xmax = xs + 200; ymax = ys + 200;
    }

    x = analogRead (pins_joy[0]);
    y = analogRead (pins_joy[1]);

    if (x < xmin)
        xmin = x; 
    else if (x > xmax)
        xmax = x;
    
    if (y < ymin)
        ymin = y; 
    else if (y > ymax)
        ymax = y;

    xf = (float)(x - xs) / ((float)(xmax - xmin) /2.0);
    yf = (float)(y - ys) / ((float)(ymax - ymin) /2.0);

    // swapped because my joystick is upside-down
    if ((yf < -0.2) && (xf <= 0.2) && (xf >= -0.2))
        pos = 2;
    else if ((xf > 0.2) && (yf <= 0.2) && (yf >= -0.2))
        pos = 3;
    else if ((yf > 0.2) && (xf <= 0.2) && (xf >= -0.2))
        pos = 0;
    else if ((xf < -0.2) && (yf <= 0.2) && (yf >= -0.2))
        pos = 1;
    else
        pos = 4;

    if (pos == pos_last)
        return false;
    
    pos_last = pos;
    return true;
}

