#ifndef STATE_H
#define STATE_H

#include "bluetooth.h"
#include "wifi.h"
#include "led.h"

typedef int state_t;

const state_t STATE_INIT = 0;       // setup, before starting to try to connect to device
const state_t STATE_CONNECTING = 1; // attempting to find and connect to desk bluetooth adapter
const state_t STATE_CONNECTED = 2;  // connected, sending commands
const state_t STATE_WAITING = 3;    // waiting for the right time to send command
const state_t STATE_ERROR = 99;     // any unrecoverable error state

state_t state = STATE_INIT;  // Overall state

void setLED() {
  switch (state) {
    case STATE_INIT: led_yellow(); break;
    case STATE_CONNECTING: led_green(); break;
    case STATE_CONNECTED: led_blue(); break;
    case STATE_WAITING: led_magenta(); break;
    case STATE_ERROR: led_red(); break;
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
  setLED();

  Serial.print("State change: ");
  writeStateToSerial(oldState);
  Serial.print(" --> ");
  writeStateToSerial(newState);
  Serial.println();

  if (state == STATE_WAITING) {
    bluetooth_off();
    wifi_on();
  } else if  (state == STATE_CONNECTING) {
    wifi_off();
    bluetooth_on();
    scan_for_device();
  }
}

#endif