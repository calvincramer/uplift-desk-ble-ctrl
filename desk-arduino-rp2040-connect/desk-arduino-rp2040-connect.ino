/*
  Uplift Desk Controll over Bluetooth

  The sketch is intended to run on the Arduino Nano RP2040 Connect
*/

#include <ArduinoBLE.h>
#include <WiFiNINA.h>

#include "led.h"
#include "config.h"
#include "state.h"
#include "time.h"
#include "desk.h"

unsigned long millis_printed_waiting_msg = 0;

// press reset or power the board
void setup() {
    Serial.begin(9600);
    // Waiting on Serial object will fail if board not connected to computer.
    // while (!Serial) { delay(10); }
    delay(1000);
    Serial.print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    setup_led();
    setState(STATE_WAITING);
}

// Runs over and over again forever
void loop() {
    led_loop();
    switch (state) {
    case STATE_ERROR:
        loop_error();
        break;
    case STATE_WAITING:
        loop_waiting();
        break;
    case STATE_CONNECTING:
        loop_connecting();
        break;
    default:
        Serial.print("Unexpected state: ");
        writeStateToSerial(state);
        Serial.println();
        delay(1000);
        break;
    }
}

void loop_error() {
    Serial.println("Unrecoverable error. Look up for error.");
    delay(1000 * 60 * 60);
}

// Wait until it's time to do things
void loop_waiting() {
    // Get current time
    get_time();
    int current_hour = get_hour_current_time_zone();

    // Start trying to run desk command?
    if (current_hour == HOUR_TO_RUN_DESK_COMMAND && ranDeskCommand == false) {
        setState(STATE_CONNECTING);
        return;
    }

    // After ran desk command after a bit reset for tomorrow
    if ((HOUR_TO_RUN_DESK_COMMAND + 2) % 24 == current_hour) {
        ranDeskCommand = false;
    }

    // Status message while waiting. Still go through the main loop() to allow LED blinking
    if (millis_printed_waiting_msg == 0 || millis() - millis_printed_waiting_msg > 1000 * 60 * 1) {
        Serial.print("Time is ");
        print_time();
        Serial.print(". Waiting until hour ");
        Serial.print(HOUR_TO_RUN_DESK_COMMAND);
        Serial.println();
        millis_printed_waiting_msg = millis();
    }
    delay(500);
}

// Try to connect and control device
void loop_connecting() {
    // Any device available from scan?
    BLEDevice device = BLE.available();
    if (device) {
        // Got a device
        BLE.stopScan();

        // Ensure it's the device we're looking for
        if (device.localName() != ADAPTER_NAME) {
            Serial.print("Error: unexpected device name: ");
            Serial.println(device.localName());
            delay(500);
            return;
        }
        printDeviceInfo(device);

        // Operate device
        controlDesk(device);
    }
}
