// based originally on:
//   SSD1306xLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x64
//   author: Neven Boyanov
// NOTE: adjust i2c_buf_s or your wire library buffer size as needed
#ifndef OLED_IDM_H
#define OLED_IDM_H

#include <Arduino.h>
#include <Wire.h>

class OLED {
  public:
    void begin ();
    void cursor_set (uint8_t x, uint8_t yp);
    void cursor_go ();
    void fill (uint8_t fill);
    void clear ();
    void rect (uint8_t x0, uint8_t yp0, uint8_t x1, uint8_t yp1, \
        uint8_t fill);
    void bitmap (uint8_t x0, uint8_t yp0, uint8_t x1, uint8_t yp1,
        const uint8_t *_data);
    void pixel_on (uint8_t x, uint8_t y);
    void pixel_off (uint8_t x, uint8_t y);
    void print (uint8_t ch);
    void setfont (uint8_t font_w, const uint8_t *_font);

    uint8_t res_x, res_yp;
    uint8_t cur_x, cur_yp;

    uint8_t font_w;
    uint8_t *_font; // PROGMEM
    
    static const uint8_t i2c_addr = 0x3c;
  // from TinyWireM - USI_BUF_SIZE set to 34 in Wire.h works well
  #ifdef USI_BUF_SIZE
    static const uint8_t i2c_buf_s = USI_BUF_SIZE - 2;
  #else
    static const uint8_t i2c_buf_s = 31;
  #endif
};

#endif

