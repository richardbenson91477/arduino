// ArduinoNunchuk.cpp - Improved Wii Nunchuk library for Arduino
// Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/
// Project URL: http://www.gabrielbianconi.com/projects/arduinonunchuk/
//// mod by richardbenson91477@gmail.com 1/30/18
#ifndef NUNCHUK_IDM_H
#define NUNCHUK_IDM_H
#include <Arduino.h>

class Nunchuk {
  public:
    static void init ();
    static void send (uint8_t data, uint8_t loc);
    void update ();
    
    int ax, ay;
    int rx, ry, rz;
    int z, c;

    static const char i2c_addr = 0x52;
};

#endif

