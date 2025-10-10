#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <ArduinoBLE.h>

#include "config.h"

// "Please note: While using the Bluetooth® Low Energy mode on the NINA module, the RGB cannot be
// used by default. While the module is in Bluetooth® Low Energy mode, SPI is deactivated, which is
// used to control the RGBs."
// https://docs.arduino.cc/tutorials/nano-rp2040-connect/rp2040-01-technical-reference/?queryID=4e5c31d3cc445c8420f020f036569c94#rgb
// This helps to setup LEDs and set their color initially. Otherwise called BLE.end() messes things
// up and WiFi is not able to connect.
bool is_bluetooth_on = false;

// Initialize bluetooth
void bluetooth_on() {
    if (is_bluetooth_on == true) {
        return;
    }
    Serial.println("Turning bluetooth on");
    while (!BLE.begin()) {
        Serial.println("Failed to start BLE library");
        while (true) {
            delay(1000);
        }
    }
    is_bluetooth_on = true;
}

// Turn bluetooth mode off. This is important to let the LED work.
void bluetooth_off() {
    if (is_bluetooth_on == false) {
        return;
    }
    Serial.println("Turning bluetooth off");
    BLE.end();
    is_bluetooth_on = false;
}

// Start bluetooth scan looking for specific device. Choose between looking for name, address, or UUID. Whatever works.
void scan_for_device() {
    Serial.println("Starting scan");
    // BLE.scanForAddress(ADAPTER_BT_ADDRESS);
    BLE.scanForName(ADAPTER_NAME);
    // BLE.scanForUuid(ADAPTER_UUID);
}

void printDeviceInfo(BLEDevice device) {
    Serial.print("Got desk bluetooth adapter! --> addr=");
    Serial.print(device.address());
    Serial.print(", name='");
    Serial.print(device.localName());
    Serial.print("', advertizeServiceUUID=");
    Serial.println(device.advertisedServiceUuid());
}

#endif