#include <Arduino.h>
#include <IRremote.h>

// my draw:
// @5.0V / 100KR / CS1N60 / R: 140-134mA G: 152-137mA B: 134-124mA W: 405-384mA
// @3.3V / 10KR / IRF840 / R: 115mA G: 117mA B: 85mA W: 262mA

// modify to taste
// NOTE: led_pins must be PWM capable, and
//   pin 11 is unusable during IR for some reason
const uint8_t leds_n = 3;
const uint8_t led_pins [] = {9, 10, 6};
const uint8_t ir_pin = 4;
const uint16_t on_sleep_len = 20;
const uint16_t on_fade_sleep_len = 20;
const uint16_t off_sleep_len = 20;
const uint8_t solid_damp_step = 5;
const uint32_t \
    IR_NONE = 0,
    IR_REPEAT = 0xFFFFFFFF,
    IR_B_I = 0xF700FF,
    IR_RG1 = 0xF720DF,
    IR_RG2 = 0xF710EF,
    IR_RG3 = 0xF730CF,
    IR_RG4 = 0xF708F7,
    IR_RG5 = 0xF728D7,
    IR_B_D = 0xF7807F,
    IR_GB1 = 0xF7A05F,
    IR_GB2 = 0xF7906F,
    IR_GB3 = 0xF7B04F,
    IR_GB4 = 0xF78877,
    IR_GB5 = 0xF7A857,
    IR_OFF = 0xF740BF,
    IR_BR1 = 0xF7609F,
    IR_BR2 = 0xF750AF,
    IR_BR3 = 0xF7708F,
    IR_BR4 = 0xF748B7,
    IR_BR5 = 0xF76897,
    IR_ON = 0xF7C03F,
    IR_WHITE = 0xF7E01F,
    IR_FLASH = 0xF7D02F,
    IR_STROBE = 0xF7F00F,
    IR_FADE = 0xF7C837,
    IR_SMOOTH = 0xF7E817;

bool on_;

const uint8_t modes_n = 5;
enum {
    MODE_NONE,
    MODE_FADE,
    MODE_BLINK_1S,
    MODE_BLINK_RND,
    MODE_SWITCH_1S,
    MODE_SWITCH_RND,
};
uint8_t mode;
uint16_t mode_t;
uint16_t mode_len;
uint8_t mode_prev;

bool auto_mode_;
uint16_t auto_mode_t;
uint16_t auto_mode_len;

bool solid_;
uint8_t solid_b [leds_n];
bool solid_damp_;
int16_t solid_damp_b;

struct led_state {
    uint8_t pin;
    uint8_t b;

    int16_t fade_cur;
    int16_t fade_stop;
    int16_t fade_step;
} leds [leds_n];

IRrecv ir_recv (ir_pin);
decode_results ir_res;

void set_mode (uint8_t mode_id) {
    mode = mode_id;
    mode_t = 0;

    if (mode == MODE_FADE) {
        if (mode_prev != MODE_FADE)
            mode_len = on_fade_sleep_len * 256;
    }
    if ((mode == MODE_BLINK_1S) ||
          (mode == MODE_SWITCH_1S)) {
        mode_len = 1000;
    }
    else if (mode == MODE_BLINK_RND) {
        if (mode_prev != mode)
            mode_len = random (200, 600);
    }
    else if (mode == MODE_SWITCH_RND) {
        if (mode_prev != mode)
        mode_len = random (400, 2000);
    }
    
    uint8_t c = 0;
    for (auto &led : leds) {
        auto _led = &led;
    
        if (mode == MODE_FADE) {
            _led->fade_stop = random (256);
            _led->fade_step = 1;
            if (_led->fade_cur > _led->fade_stop)
                _led->fade_step = -_led->fade_step;

            _led->b = _led->fade_cur;
        }
        else if ((mode == MODE_BLINK_1S) ||
              (mode == MODE_BLINK_RND)) {
            if (_led->b > 0)
                _led->b = 0;
            else if (solid_) {
                if (solid_b[c] + solid_damp_b < 1)
                    _led->b = 1;
                else if (solid_b[c] + solid_damp_b > 0xff)
                    _led->b = 0xff;
                else
                    _led->b = solid_b[c] + solid_damp_b;
            }
            else
                _led->b = random (16, 256);
        }
        else if ((mode == MODE_SWITCH_1S) ||
              (mode == MODE_SWITCH_RND)) {
            if (solid_) {
                if (solid_b[c] + solid_damp_b < 1)
                    _led->b = 1;
                else if (solid_b[c] + solid_damp_b > 0xff)
                    _led->b = 0xff;
                else
                    _led->b = solid_b[c] + solid_damp_b;
            }
            else
                _led->b = random (256);
        }

        analogWrite (_led->pin, _led->b);
        c ++;
    }

    mode_prev = mode;
}

void tick_mode (uint16_t t) {
    if (auto_mode_) {
        auto_mode_t += t;
        if (auto_mode_t >= auto_mode_len) {
            auto_mode_t = 0;
            set_mode (random (modes_n) + 1);
        }
    }
    
    mode_t += t;
    if (mode_t >= mode_len) {
        set_mode (mode);
    }

    for (auto &led : leds) {
        auto _led = &led;

        if (mode == MODE_FADE) {
            _led->fade_cur += _led->fade_step;
            if (((_led->fade_cur > _led->fade_stop) &&
                  (_led->fade_step > 0)) ||
                  ((_led->fade_cur < _led->fade_stop) &&
                  (_led->fade_step < 0))) {
                _led->fade_step = 0;
                _led->fade_cur = _led->fade_stop;
            }

            _led->b = _led->fade_cur;
        }
        
        analogWrite (_led->pin, _led->b);
    }
}

void set_solid (uint8_t b1, uint8_t b2, uint8_t b3) {
    for (uint8_t c = 0; c < leds_n; c ++) {
        if (c % 3 == 0)
            solid_b[c] = b1;
        else if (c % 3 == 1)
            solid_b[c] = b2;
        else if (c % 3 == 2)
            solid_b[c] = b3;
    }
    solid_damp_b = 0;
}

void enable_solid (uint32_t val) {
    solid_ = true;

    if (val == IR_B_I) {
        if (! solid_damp_)
            solid_damp_ = true;

        for (uint8_t c = 0; c < leds_n; c ++) {
            if (solid_damp_b + solid_damp_step > 254)
                solid_damp_b = 254;
            else 
                solid_damp_b += solid_damp_step;
        }
    }
    else if (val == IR_B_D) {
        if (! solid_damp_)
            solid_damp_ = true;

        for (uint8_t c = 0; c < leds_n; c ++) {
            if (solid_damp_b - solid_damp_step < -254)
                solid_damp_b = -254;
            else 
                solid_damp_b -= solid_damp_step;
        }
    }
    else if (val == IR_WHITE)
        set_solid (0xff, 0xff, 0xff);
    else if (val == IR_RG1)
        set_solid (0xff, 0, 0);
    else if (val == IR_RG2)
        set_solid (0xff, 0x10, 0);
    else if (val == IR_RG3)
        set_solid (0xff, 0x20, 0);
    else if (val == IR_RG4)
        set_solid (0xff, 0x30, 0);
    else if (val == IR_RG5)
        set_solid (0xff, 0x60, 0);
    else if (val == IR_GB1)
        set_solid (0, 0xff, 0);
    else if (val == IR_GB2)
        set_solid (0, 0xff, 0x22);
    else if (val == IR_GB3)
        set_solid (0, 0xff, 0x44);
    else if (val == IR_GB4)
        set_solid (0, 0xff, 0x66);
    else if (val == IR_GB5)
        set_solid (0, 0xe0, 0x80);
    else if (val == IR_BR1)
        set_solid (0, 0, 0xff);
    else if (val == IR_BR2)
        set_solid (0x38, 0, 0xff);
    else if (val == IR_BR3)
        set_solid (0x70, 0, 0xff);
    else if (val == IR_BR4)
        set_solid (0xa8, 0, 0xff);
    else if (val == IR_BR5)
        set_solid (0xff, 0, 0xff);
}

void tick_ir () {
    static uint32_t val_prev = IR_NONE;

    if (! ir_recv.decode (&ir_res))
        return;

    if (ir_res.decode_type != NEC)
        return;

    uint32_t val = ir_res.value;
    ir_recv.resume ();

    if ((! on_) && (val != IR_ON))
        return;

    if (val == IR_REPEAT)
        val = val_prev;

    if ((val == IR_RG1) || (val == IR_RG2) || (val == IR_RG3) ||
          (val == IR_RG4) || (val == IR_RG5) || (val == IR_GB1) ||
          (val == IR_GB2) || (val == IR_GB3) || (val == IR_GB4) ||
          (val == IR_GB5) || (val == IR_BR1) || (val == IR_BR2) ||
          (val == IR_BR3) || (val == IR_BR4) || (val == IR_BR5) ||
          (val == IR_WHITE) || (val == IR_B_I) || (val == IR_B_D)) {
        enable_solid (val);
        auto_mode_ = false;
        if (mode == MODE_FADE)
            set_mode (MODE_SWITCH_1S);
        else
            set_mode (mode);
        auto_mode_ = false;
    }
    else if (val == IR_ON) {
        if (on_) {
            if (! auto_mode_) {
                auto_mode_ = true;
                auto_mode_t = 0;
                solid_ = false;
            }
        }
        else {
            on ();
        }
    }
    else if (val == IR_OFF) {
        off ();
    }
    else if (val == IR_FLASH) {
        set_mode (MODE_BLINK_1S);
        auto_mode_ = false;
    }
    else if (val == IR_STROBE) {
        mode_prev = MODE_NONE;
        set_mode (MODE_BLINK_RND);
        auto_mode_ = false;
    }
    else if (val == IR_FADE) {
        set_mode (MODE_FADE);
        auto_mode_ = false;
    }
    else if (val == IR_SMOOTH) {
        set_mode (MODE_SWITCH_1S);
        auto_mode_ = false;
    }
    
    val_prev = val;
}

void on () {
    on_ = true;

    mode = mode_prev = MODE_NONE;
    mode_t = 0;
    mode_len = 1000;
    auto_mode_ = true;
    auto_mode_t = 0;
    auto_mode_len = 12000;
    solid_ = false;
    solid_damp_ = false;

    set_mode (random (modes_n) + 1);
}

void off () {
    on_ = false;

    for (auto &led : leds) {
        auto _led = &led;
        digitalWrite (_led->pin, LOW);
    }
}

void setup () {
    randomSeed (analogRead (0));

    for (uint8_t c = 0; c < leds_n; c ++) {
        leds[c].pin = led_pins[c];
        pinMode (led_pins[c], OUTPUT);
        leds[c].b = leds[c].fade_cur = 0xa0;
    }
 
    pinMode (ir_pin, INPUT);
    ir_recv.enableIRIn ();

    on ();
}

void loop () {
    static uint32_t tick_prev = millis ();

    uint32_t tick_t = millis () - tick_prev;
    tick_prev = millis ();
    if (tick_t < on_sleep_len)
        tick_t = on_sleep_len;

    if (on_) {
        tick_mode (tick_t);
        tick_ir ();
        if (mode == MODE_FADE)
            delay (on_fade_sleep_len);
        else
            delay (on_sleep_len);
    }
    else {
        tick_ir ();
        delay (off_sleep_len);
    }
}

