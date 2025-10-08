#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <ArduinoBLE.h>

#include "config.h"

// Initialize bluetooth
void bluetooth_on() {
    Serial.println("Turning bluetooth on");
    while (!BLE.begin()) {
        Serial.println("Failed to start BLE library");
        while (true) {
            delay(1000);
        }
    }
}

void bluetooth_off() {
    Serial.println("Turning bluetooth off");
    BLE.end();
}

// Start bluetooth scan
void scan_for_device() {
    Serial.println("Starting scan");
    // BLE.scanForAddress(ADAPTER_BT_ADDRESS);
    BLE.scanForName(ADAPTER_NAME);
    // BLE.scanForUuid(ADAPTER_UUID);
    // Debugging: scan for everything
    // BLE.scan();
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