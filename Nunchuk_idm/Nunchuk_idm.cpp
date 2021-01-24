// ArduinoNunchuk.cpp - Improved Wii Nunchuk library for Arduino
// Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/
// Project URL: http://www.gabrielbianconi.com/projects/arduinonunchuk/
//// mod by richardbenson91477@protonmail.com 1/30/18
#include <Arduino.h>
#include <Wire.h>
#include "Nunchuk_idm.h"

// class static
void Nunchuk::init () {
    Wire.begin ();
    Nunchuk::send (0x55, 0xF0);
    Nunchuk::send (0x00, 0xFB);
}

// class static
void Nunchuk::send (uint8_t data, uint8_t loc) {
    Wire.beginTransmission (Nunchuk::i2c_addr);
    Wire.write (loc);
    Wire.write (data);
    Wire.endTransmission ();
    delay (10);
}

void Nunchuk::update () {
    int vals [6];
    int c = 0;

    Wire.requestFrom (Nunchuk::i2c_addr, 6);
    while (Wire.available ()) {
//        vals[c] = Wire.receive ();
        vals[c] = Wire.read ();
        // ? vals[c] = 0x17 + (0x17 ^ vals[c];
        c ++;
    }

    this->ax = vals[0];
    this->ay = vals[1];
    this->rx = (vals[2] << 2) | ((vals [5] >> 2) & 3);
    this->ry = (vals[3] << 2) | ((vals [5] >> 4) & 3);
    this->rz = (vals[4] << 2) | ((vals [5] >> 6) & 3);
    this->z = !(vals[5] & 1);
    this->c = !(vals[5] & 2);

    Nunchuk::send (0x00, 0x00);
}

