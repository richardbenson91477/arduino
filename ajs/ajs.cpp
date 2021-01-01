#include <Arduino.h>
// using Joystick library by Matthew Heironimus
#include <Joystick.h>

#ifndef PIN_X
    #define PIN_X A3
#endif
#ifndef PIN_A
    #define PIN_A 8
#endif
#ifndef PIN_B
    #define PIN_B 9
#endif

#ifndef DELAY_INACTIVE
    #define DELAY_NOX 22
#endif

#ifndef DELAY_ACTIVE
    #define DELAY_X 8
#endif

Joystick_ js (JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
  2, 0,                  // Buttons, Hat Switches
  true, true, false,     // X, Y Z
  false, false, false,   // Rx, Ry, or Rz
  false, false,          // rudder, throttle
  false, false, false);  // accelerator, brake, steering

void setup () {
    //Serial.begin(9600);
    pinMode(PIN_X, INPUT);
    pinMode(PIN_A, INPUT_PULLUP);
    pinMode(PIN_B, INPUT_PULLUP);
    js.begin(false);
}

void loop () {
    static int16_t x_prev = 0, a_prev = 0, b_prev = 0;
    
    //Serial.println(x, a, b);
   
    int16_t a = ! digitalRead(PIN_A);
    if (a != a_prev) {
        a_prev = a;
        js.setButton(0, a);
        js.sendState();
        
    }
    int16_t b = ! digitalRead(PIN_B);
    if (b != b_prev) {
        b_prev = b;
        js.setButton(1, b);
        js.sendState();
    }
    
    int16_t x = analogRead(PIN_X);
    if (x != x_prev) {
        x_prev = x;
        js.setXAxis(x);
        js.sendState();
        delay(DELAY_X);
        return;
    }

    delay(DELAY_NOX);
}
