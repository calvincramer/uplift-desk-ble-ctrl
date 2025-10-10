#include "api/Common.h"
#ifndef LED_H
#define LED_H

#include <WiFiNINA.h>

#include "bluetooth.h"

// Arduino Nano RP2040 Connect LEDs:
// 1. RGB LED (DL3). Connected to ublox nina w102 GPIO. Near the reset button
//        OFF when digital state is HIGH, ON when digital state is LOW
// 2. Common Anode LED. Where?
// 3. Green power on LED. Builtin not controllable I think
// 4. Orange LED network. Builtin not controllable I think

PinStatus _led_state_r = LOW;
PinStatus _led_state_g = LOW;
PinStatus _led_state_b = LOW;

bool _led_is_setup = false;

// Set LED pins to desired LED color. Blink depending on time.
void _update_led_pins() {
    if (is_bluetooth_on == true) {
        return;
    }
    if ((millis() / 1000) % 2 == 0) {
        // Blink on
        digitalWrite(LEDR, _led_state_r);
        digitalWrite(LEDG, _led_state_g);
        digitalWrite(LEDB, _led_state_b);
    } else {
        // Blink off
        digitalWrite(LEDR, LOW);
        digitalWrite(LEDG, LOW);
        digitalWrite(LEDB, LOW);
    }
}

void led_off() {
    _led_state_r = LOW;
    _led_state_g = LOW;
    _led_state_b = LOW;
    _update_led_pins();
}

void led_red() {
    _led_state_r = HIGH;
    _led_state_g = LOW;
    _led_state_b = LOW;
    _update_led_pins();
}

void led_green() {
    _led_state_r = LOW;
    _led_state_g = HIGH;
    _led_state_b = LOW;
    _update_led_pins();
}

void led_blue() {
    _led_state_r = LOW;
    _led_state_g = LOW;
    _led_state_b = HIGH;
    _update_led_pins();
}

void led_yellow() {
    _led_state_r = HIGH;
    _led_state_g = HIGH;
    _led_state_b = LOW;
    _update_led_pins();
}

void led_cyan() {
    _led_state_r = LOW;
    _led_state_g = HIGH;
    _led_state_b = HIGH;
    _update_led_pins();
}

void led_magenta() {
    _led_state_r = HIGH;
    _led_state_g = LOW;
    _led_state_b = HIGH;
    _update_led_pins();
}

void led_white() {
    _led_state_r = HIGH;
    _led_state_g = HIGH;
    _led_state_b = HIGH;
    _update_led_pins();
}

void led_loop() {
    _update_led_pins();
}

void setup_led() {
    if (_led_is_setup == true) {
        return;
    }
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    led_off();
    _led_is_setup = true;
}

void teardown_led() {
    if (_led_is_setup == false) {
        return;
    }
    led_off();
    // pinMode(LEDR, OUTPUT_OPENDRAIN);
    // pinMode(LEDG, OUTPUT_OPENDRAIN);
    // pinMode(LEDB, OUTPUT_OPENDRAIN);
    _led_is_setup = false;
}

#endif