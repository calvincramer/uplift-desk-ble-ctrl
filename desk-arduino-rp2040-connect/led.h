#ifndef LED_H
#define LED_H

#include <WiFiNINA.h>

// const int WIFI_RED_PIN = 26;
// const int WIFI_GREEN_PIN = 25;
// const int WIFI_BLUE_PIN = 27;

void setup_led() {
  // WTF LEDs...
  // For some reason trying to use LEDs makes WiFi fail.

  // WiFiDrv::pinMode(18, OUTPUT); // Red
  // WiFiDrv::pinMode(16, OUTPUT); // Green
  // WiFiDrv::pinMode(17, OUTPUT); // Blue

  // pinMode(LED_BUILTIN, OUTPUT);

  // pinMode(LEDR, OUTPUT);
  // pinMode(LEDG, OUTPUT);
  // pinMode(LEDB, OUTPUT);

  // digitalWrite(LEDR, LOW);
  // digitalWrite(LEDG, LOW);
  // digitalWrite(LEDB, LOW);
}

void led_off() {
  return;
  digitalWrite(LEDR, LOW); 
  digitalWrite(LEDG, LOW); 
  digitalWrite(LEDB, LOW);
}

void led_red() {
  return;
  digitalWrite(LEDR, HIGH); 
  digitalWrite(LEDG, LOW); 
  digitalWrite(LEDB, LOW);
}

void led_green() {
  return;
  digitalWrite(LEDR, LOW); 
  digitalWrite(LEDG, HIGH); 
  digitalWrite(LEDB, LOW);
}

void led_blue() {
  return;
  digitalWrite(LEDR, LOW); 
  digitalWrite(LEDG, LOW); 
  digitalWrite(LEDB, HIGH);
}

void led_yellow() {
  return;
  digitalWrite(LEDR, HIGH); 
  digitalWrite(LEDG, HIGH); 
  digitalWrite(LEDB, LOW);
}

void led_cyan() {
  return;
  digitalWrite(LEDR, LOW); 
  digitalWrite(LEDG, HIGH); 
  digitalWrite(LEDB, HIGH);
}

void led_magenta() {
  return;
  digitalWrite(LEDR, HIGH); 
  digitalWrite(LEDG, LOW); 
  digitalWrite(LEDB, HIGH);
}

void led_white() {
  return;
  digitalWrite(LEDR, HIGH); 
  digitalWrite(LEDG, HIGH); 
  digitalWrite(LEDB, HIGH);
}

#endif