#include "utility/wl_definitions.h"
#ifndef WIFI_H
#define WIFI_H

#include <WiFiNINA.h>
#include <WiFiUdp.h>

#include "state.h"

void wifi_on() {
  Serial.println("Turning on WiFi and connecting");
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true) {
      delay(100000);
    }
  }
  if (WiFi.firmwareVersion() < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the wifi firmware");
  }

  int wifi_status = WL_IDLE_STATUS;
  while (wifi_status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    wifi_status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.print("Connected to WiFi! ");
  Serial.print(WiFi.SSID());
  Serial.print(" ");
  Serial.print(WiFi.localIP());
  Serial.print(" ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

void wifi_off() {
  Serial.println("Turning off wifi");
  WiFi.disconnect();
  WiFi.end();
}

unsigned long get_time_from_ntp() {
  Serial.println("Getting time from WifFi.getTime()... ");
  return WiFi.getTime();
}

#endif