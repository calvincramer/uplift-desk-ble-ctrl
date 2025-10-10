#ifndef STATE_H
#define STATE_H

#include "wifi.h"
#include "bluetooth.h"
#include "led.h"

typedef int state_t;

const state_t STATE_INIT = 0;       // setup, before starting to try to connect to device
const state_t STATE_CONNECTING = 1; // attempting to find and connect to desk bluetooth adapter
const state_t STATE_CONNECTED = 2;  // connected, sending commands
const state_t STATE_WAITING = 3;    // waiting for the right time to send command
const state_t STATE_ERROR = 99;     // any unrecoverable error state

state_t state = STATE_INIT; // Overall state

void setLED() {
    switch (state) {
    case STATE_INIT:
        led_yellow();
        break;
    case STATE_CONNECTING:
        // led_green();  // LED not accesible while using BLE
        break;
    case STATE_CONNECTED:
        // led_blue();  // LED not accesible while using BLE
        break;
    case STATE_WAITING:
        led_magenta();
        break;
    case STATE_ERROR:
        led_red();
        break;
    default:
        led_red();
        Serial.write("ERROR: UNKNOWN STATE ");
        Serial.write(state);
        Serial.println();
        break;
    }
}

void writeStateToSerial(state_t s) {
    if (s == STATE_INIT) {
        Serial.print("STATE_INIT");
    } else if (s == STATE_CONNECTING) {
        Serial.print("STATE_CONNECTING");
    } else if (s == STATE_CONNECTED) {
        Serial.print("STATE_CONNECTED");
    } else if (s == STATE_WAITING) {
        Serial.print("STATE_WAITING");
    } else if (s == STATE_ERROR) {
        Serial.print("STATE_ERROR");
    } else {
        Serial.print("UNKNOWN STATE");
    }
}

void setState(state_t newState) {
    if (state == newState) {
        return;
    }

    state_t oldState = state;
    state = newState;

    Serial.print("State change: ");
    writeStateToSerial(oldState);
    Serial.print(" --> ");
    writeStateToSerial(newState);
    Serial.println();

    // Warning: LED not usable when in BLE mode
    if (state == STATE_WAITING) {
        bluetooth_off();
        delay(100);
        wifi_on();
        delay(100);
        setup_led();
        delay(100);
    } else if (state == STATE_CONNECTING) {
        teardown_led();
        delay(100);
        wifi_off();
        delay(100);
        bluetooth_on();
        delay(100);
        scan_for_device();
        delay(100);
    }

    setLED();
}

#endif