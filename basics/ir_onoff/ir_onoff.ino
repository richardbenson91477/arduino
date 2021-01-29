#include <Arduino.h>
#include <IRLib.h>

const uint8_t PIN_LED = 13;
const uint32_t IR_OFF = 0xF740BF;
const uint32_t IR_ON = 0xF7C03F;

IRrecv My_Receiver (4);
IRdecode My_Decoder; 

void setup () { 
    Serial.begin (9600);
    My_Receiver.enableIRIn ();
    pinMode (PIN_LED, OUTPUT);
} 

void loop() { 
    if (My_Receiver.GetResults (&My_Decoder)) {
        My_Decoder.decode ();
        if (My_Decoder.decode_type == NEC) {
            auto val = My_Decoder.value;    
            Serial.println (val);
            if (val == IR_OFF)
                digitalWrite (PIN_LED, LOW);
            else if (val == IR_ON)
                digitalWrite (PIN_LED, HIGH);
        }

        My_Receiver.resume ();
    }

    delay (20);
}

