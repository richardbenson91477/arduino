// based originally on:
//   SSD1306xLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x64
//   author: Neven Boyanov
#ifndef SSD1306_INIT_SEQ_H
#define SSD1306_INIT_SEQ_H
#include <Arduino.h>

const uint8_t SSD1306_init_seq [] PROGMEM = {
  0xae,         // display on
  0xd5, 0x80,   // display clock divide ratio/oscillator frequency
  0xa8, 0x3f,   // multiplex ratio (1 to 64)
  0xd3, 0x00,   // display offset
  0x40,         // start line address
  0x8d, 0x14,   // DC-DC enable
  0x20, 0x00,   // memory addressing mode
  0xa1,         // segment re-map. a0 = address mapped; a1=address 127 mapped
  0xc8,         // COM output scan direction c8 = dec, inc = c0
  0xda, 0x12,   // com pins hardware configuration
  0x81, 0xcf,   // contrast control register
  0xd9, 0xf1,   // pre-charge period
  0xdb, 0x40,   // vcomh: 0x20 = 0.77xVcc
  0xa4,         // DISPLAYALLON_RESUME
  0xa6,         // 0xa6 = normal display; 0xa7 = inverse display
  0x2e,         // stop scroll
  0xaf          // display ON in normal mode
};

#endif

