#ifndef DESK_H
#define DESK_H

#include "state.h"

// Desk control bluetooth payloads
const uint8_t CMD_PACKET_WAKE[] = {0xF1, 0xF1, 0x00, 0x00, 0x00, 0x7E};
const uint8_t CMD_PACKET_GO_TO_PRESET_2[] = {0xF1, 0xF1, 0x06, 0x00, 0x06, 0x7E};

bool ranDeskCommand = false;  // true after we send command to raise. Resets to false after a few hours.

// Steps:
// 1. Connect to the desk over bluetooth
// 2. Discover bluetooth attributes (this is necessary)
// 3. Get expected bluetooth characteristic foir desk control
// 4. Wake bluetooth adapter (necessary)
// 5. Send user command
// 6. Go back to WAITING state
void controlDesk(BLEDevice device) {
    // Connect
    Serial.print("Connecting...");
    bool connected = false;
    for (int tryCount = 1; tryCount <= 6; tryCount++) {
      if (device.connect()) {
        Serial.println(" connected!");
        connected = true;
        break;
      } else {
        Serial.print(" fail");
      }
    }
    if (connected == false) {
      Serial.println();
      Serial.println("Failed to connect");
      return;
    }
    setState(STATE_CONNECTED);

    // Discover attributes
    Serial.println("Discovering attributes ...");
    if (device.discoverAttributes()) {
      Serial.println("Attributes discovered");
    } else {
      Serial.println("Attribute discovery failed!");
      device.disconnect();
      return;
    }

    // Get desk control characteristic
    BLECharacteristic charDeskCtrl = device.characteristic(CHAR_UUID_DESK_CTRL);
    if (!charDeskCtrl) {
      Serial.println("Bluetooth adapter does not have desk control characteristic");
      device.disconnect();
      return;
    }
    
    // Write command
    if (charDeskCtrl.canWrite() == false) {
      Serial.println("Desk control characteristic not writable");
      device.disconnect();
      return;
    }

    // Wake
    Serial.println("Sending command: waking");
    if (!charDeskCtrl.writeValue(CMD_PACKET_WAKE, sizeof(CMD_PACKET_WAKE))) {
      Serial.println("Failed sending command to wake");
      device.disconnect();
      return;
    }

    // Go to preset 2
    Serial.println("Sending command: Go to height preset 2");
    if (!charDeskCtrl.writeValue(CMD_PACKET_GO_TO_PRESET_2, sizeof(CMD_PACKET_GO_TO_PRESET_2))) {
      Serial.println("Failed sending command to go to height preset 2");
      device.disconnect();
      return;
    }
  
    device.disconnect();
    ranDeskCommand = true;
    setState(STATE_WAITING);
}

#endif