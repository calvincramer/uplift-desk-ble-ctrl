#include "utility/wl_definitions.h"
#ifndef WIFI_H
#define WIFI_H

#include <WiFiNINA.h>
#include <WiFiUdp.h>

#include "config-private.h"

void wifi_on() {
    Serial.println("Turning on WiFi and connecting");
    while (true) {
        uint8_t status = WiFi.status();
        if (WiFi.status() == WL_NO_MODULE) {
            Serial.println("Communication with WiFi module failed! (status=WL_NO_MODULE)");
            delay(1000);
            continue;
        }
        break;
    }

    if (WiFi.firmwareVersion() < WIFI_FIRMWARE_LATEST_VERSION) {
        Serial.println("Please upgrade the wifi firmware");
    }

    int wifi_status = WL_IDLE_STATUS;
    int attempt_count = 0;
    while (wifi_status != WL_CONNECTED) {
        if (attempt_count % 10 == 0) {
            if (attempt_count > 0) {
                Serial.println();
            }
            Serial.print("Attempting to connect to WiFi SSID: ");
            Serial.print(ssid);
            Serial.print(" ");
        } else {
            Serial.print(".");
        }
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        wifi_status = WiFi.begin(ssid, pass);
        // wait for connection:
        delay(100);
        attempt_count += 1;
    }
    Serial.println();
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
    delay(100);
    WiFi.disconnect();
    delay(100);
    WiFi.end();
    delay(100);
}

unsigned long get_time_from_ntp() {
    Serial.print("Getting time from WifFi.getTime() ");
    while (true) {
        unsigned long time = WiFi.getTime();
        // 0 is failure
        if (time != 0) {
            Serial.println();
            Serial.print("Got time: ");
            Serial.println(time);
            return time;
        }
        Serial.print(".");
        delay(1000);
    }
}

#endif