// based originally on:
//   SSD1306xLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x64
//   author: Neven Boyanov
// richardbenson91477@protonmail.com 1/30/18

#include <Arduino.h>
#include "OLED_idm.h"
#include "SSD1306_init_seq.h"

void OLED::begin () {
    uint8_t *_seq = (uint8_t *) SSD1306_init_seq;

    Wire.begin ();
    for (uint8_t i = 0; i < sizeof(SSD1306_init_seq); i++) {
        Wire.beginTransmission (OLED::i2c_addr);
        Wire.write (0x00);
        Wire.write (pgm_read_byte (_seq ++));
        Wire.endTransmission ();
    }

    // NOTE: modify this to 132 for SH1106
    this->res_x = 128;
    this->res_yp = 8;
}

void OLED::cursor_set (uint8_t x, uint8_t yp) {
    this->cur_x = x;
    this->cur_yp = yp;
    cursor_go ();
}

void OLED::cursor_go () {
    Wire.beginTransmission (OLED::i2c_addr);
    Wire.write (0x00);
    Wire.write (0xb0 + this->cur_yp);
    Wire.write (0x10 + (this->cur_x >> 4));
    Wire.write (this->cur_x & 0x0f);
    Wire.endTransmission ();
}

void OLED::fill (uint8_t fill) {
    uint16_t bs = this->res_x * this->res_yp; 
    uint8_t bdiv = bs / OLED::i2c_buf_s,
        brem = bs % OLED::i2c_buf_s;

    cursor_set (0, 0);
    for (uint8_t bdc = 0; bdc < bdiv; bdc ++) {
        Wire.beginTransmission (OLED::i2c_addr);
        Wire.write (0x40);
        for (uint8_t bc = 0; bc < OLED::i2c_buf_s; bc ++)
            Wire.write (fill);
        Wire.endTransmission ();
    }
    if (brem) {
        Wire.beginTransmission (OLED::i2c_addr);
        Wire.write (0x40);
        for (uint8_t bc = 0; bc < brem; bc ++)
            Wire.write (fill);
        Wire.endTransmission ();
    }
    cursor_go ();
}

void OLED::clear () {
    fill (0x00);
}

void OLED::rect (uint8_t x0, uint8_t yp0, uint8_t x1, uint8_t yp1, \
        uint8_t fill) {
    uint8_t xdiv = (x1 - x0) / OLED::i2c_buf_s,
        xrem = (x1 - x0) % OLED::i2c_buf_s;

    for (uint8_t ypc = yp0; ypc < yp1; ypc ++) {
        cursor_set (x0, ypc);
        for (uint8_t xdc = 0; xdc < xdiv; xdc ++) {
            Wire.beginTransmission (OLED::i2c_addr);
            Wire.write (0x40);
            for (uint8_t xc = 0; xc < OLED::i2c_buf_s; xc ++)
                Wire.write (fill);
            Wire.endTransmission ();
        }
        if (xrem) {
            Wire.beginTransmission (OLED::i2c_addr);
            Wire.write (0x40);
            for (uint8_t xc = 0; xc < xrem; xc ++)
                Wire.write (fill);
            Wire.endTransmission ();
        }
    }
}

void OLED::bitmap (uint8_t x0, uint8_t yp0, uint8_t x1, uint8_t yp1, \
        const uint8_t *_data) {
    uint8_t xdiv = (x1 - x0) / OLED::i2c_buf_s,
        xrem = (x1 - x0) % OLED::i2c_buf_s;
    uint8_t *_d2 = (uint8_t *) _data;

    for (uint8_t ypc = yp0; ypc < yp1; ypc ++) {
        cursor_set (x0, ypc);
        for (uint8_t xdc = 0; xdc < xdiv; xdc ++) {
            Wire.beginTransmission (OLED::i2c_addr);
            Wire.write (0x40);
            for (uint8_t xc = 0; xc < OLED::i2c_buf_s; xc ++)
                Wire.write (pgm_read_byte (_d2 ++));
            Wire.endTransmission ();
        }
        if (xrem) {
            Wire.beginTransmission (OLED::i2c_addr);
            Wire.write (0x40);
            for (uint8_t xc = 0; xc < xrem; xc ++)
                Wire.write (pgm_read_byte (_d2 ++));
            Wire.endTransmission ();
        }
    }
}

// TODO: implement optional READ to preserve stick
void OLED::pixel_on (uint8_t x, uint8_t y) {
    Wire.beginTransmission (OLED::i2c_addr);
    Wire.write (0x00);
    Wire.write (0xb0 + (y >> 3));
    Wire.write (0x10 + (x >> 4));
    Wire.write (x & 0x0f);
    Wire.endTransmission ();

    Wire.beginTransmission (OLED::i2c_addr);
    Wire.write (0x40);
    Wire.write (1 << (y % 8));
    Wire.endTransmission ();
}

// TODO: implement optional READ to preserve stick
void OLED::pixel_off (uint8_t x, uint8_t y) {
    Wire.beginTransmission (OLED::i2c_addr);
    Wire.write (0x00);
    Wire.write (0xb0 + (y >> 3));
    Wire.write (0x10 + (x >> 4));
    Wire.write (x & 0x0f);
    Wire.endTransmission ();

    Wire.beginTransmission (OLED::i2c_addr);
    Wire.write (0x40);
    Wire.write (0);
    Wire.endTransmission ();
}

void OLED::print (uint8_t ch) {
    uint8_t xdiv = this->font_w / i2c_buf_s,
        xrem = this->font_w % i2c_buf_s;
    uint8_t *_ch = (uint8_t *) this->_font + (ch * this->font_w);

    for (uint8_t xdc = 0; xdc < xdiv; xdc ++) {
        Wire.beginTransmission (OLED::i2c_addr);
        Wire.write (0x40);
        for (uint8_t xc = 0; xc < i2c_buf_s; xc ++)
            Wire.write (pgm_read_byte (_ch++));
        Wire.endTransmission ();
    }
    if (xrem) {
        Wire.beginTransmission (OLED::i2c_addr);
        Wire.write (0x40);
        for (uint8_t xc = 0; xc < xrem; xc ++)
            Wire.write (pgm_read_byte (_ch++));
        Wire.endTransmission ();
    }
    Wire.endTransmission ();
}
    
void OLED::setfont (uint8_t font_w, const uint8_t *_font) {
    this->font_w = font_w;
    this->_font = (uint8_t *) _font;
}

