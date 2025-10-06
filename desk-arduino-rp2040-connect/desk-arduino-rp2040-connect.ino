/*
  Uplift Desk Controll over Bluetooth

  The sketch is intended to run on the Arduino Nano RP2040 Connect

  TODO: quicker scan, so slow, fails
  TODO: raise at certain time: https://docs.arduino.cc/tutorials/communication/wifi-nina-examples/
*/

#include <WiFiNINA.h>
#include <ArduinoBLE.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// UUID of your bluetooth adapter.
const arduino::String ADAPTER_UUID = "000000ff-0000-1000-8000-00805f9b34fb";
// Name of your bluetooth adapter.
const arduino::String ADAPTER_NAME = "BLE Device 88DF2C";
// Bluetooth address of your bluetooth adapter.
const arduino::String ADAPTER_BT_ADDRESS = "f8:6d:c8:88:df:2c";

// Characteristic UUID for the main desk control. This changes depending on adapter revision.
// Whole UUID normally looks like "0000xxxx-0000-1000-8000-00805f9b34fb". Arduino only uses the 4 changing hex digits.
const char* CHAR_UUID_DESK_CTRL = "ff01";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const uint8_t CMD_PACKET_WAKE[] = {0xF1, 0xF1, 0x00, 0x00, 0x00, 0x7E};
const uint8_t CMD_PACKET_GO_TO_PRESET_2[] = {0xF1, 0xF1, 0x06, 0x00, 0x06, 0x7E};

// const int WIFI_RED_PIN = 26;
// const int WIFI_GREEN_PIN = 25;
// const int WIFI_BLUE_PIN = 27;

const int NUM_FAIL_SCAN_RETRY = 10;

const int STATE_INIT = 0;       // setup, before starting to try to connect to device
const int STATE_CONNECTING = 1; // attempting to find and connect to desk bluetooth adapter
const int STATE_CONNECTED = 2;  // connected, sending commands
const int STATE_WAITING = 3;    // waiting for the right time to send command
const int STATE_ERROR = 99;     // any unrecoverable error state

int state = STATE_INIT;  // Overall state
int scanFailCount = 0;   // number of times in main loop not find the right device

void setLED() {
  // if (state == STATE_INIT) {
  //   digitalWrite(LEDR, HIGH); 
  //   digitalWrite(LEDG, HIGH); 
  //   digitalWrite(LEDB, LOW);
  // } else if (state == STATE_ERROR) {
  //   digitalWrite(LEDR, HIGH);
  //   digitalWrite(LEDG, LOW); 
  //   digitalWrite(LEDB, LOW);
  // } else {
  //   digitalWrite(LEDR, HIGH); 
  //   digitalWrite(LEDG, LOW); 
  //   digitalWrite(LEDB, LOW);
  //   Serial.write("ERROR: UNKNOWN STATE ");
  //   Serial.write(state);
  //   Serial.println();
  // }
}

void setState(int newState) {
  state = newState;
  // setLED();
}

void writeStateToSerial() {
  if (state == STATE_INIT) {
    Serial.write("STATE_INIT");
  } else if (state == STATE_CONNECTING) {
    Serial.write("STATE_CONNECTING");
  } else if (state == STATE_CONNECTED) {
    Serial.write("STATE_CONNECTED");
  } else if (state == STATE_WAITING) {
    Serial.write("STATE_WAITING");
  } else if (state == STATE_ERROR) {
    Serial.write("STATE_ERROR");
  } else {
    Serial.write("UNKNOWN STATE");
  }
}

void scanForDevice() {
  Serial.println("Starting scan");
  BLE.scanForAddress(ADAPTER_BT_ADDRESS);
  // Debugging: scan for everything
  // BLE.scan();       
}

void noDevice() {
  Serial.print(".");
  delay(1000);
  scanFailCount += 1;
  if (scanFailCount >= NUM_FAIL_SCAN_RETRY) {
    Serial.println(" Restarting scan");
    scanFailCount = 0;
    BLE.stopScan();
    delay(500);
    scanForDevice();
  }
}

void printDeviceInfo(BLEDevice device) {
  Serial.print("Got desk bluetooth adapter! --> addr=");
  Serial.print(device.address());
  Serial.print(", name='");
  Serial.print(device.localName());
  Serial.print("', advertizeServiceUUID=");
  Serial.println(device.advertisedServiceUuid());
}

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
  
    setState(STATE_WAITING);
    device.disconnect();
}

// press reset or power the board
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }

  setState(STATE_INIT);

  // WTF LEDs...
  // WiFiDrv::pinMode(25, OUTPUT); // Red
  // WiFiDrv::pinMode(26, OUTPUT); // Green
  // WiFiDrv::pinMode(27, OUTPUT); // Blue
  // pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(LEDR, OUTPUT);
  // pinMode(LEDG, OUTPUT);
  // pinMode(LEDB, OUTPUT);
  // digitalWrite(LEDR, LOW);
  // digitalWrite(LEDG, LOW);
  // digitalWrite(LEDB, LOW);

  Serial.println("Starting BLE library");
  if (!BLE.begin()) {
    setState(STATE_ERROR);
    Serial.println("Failed to start BLE library");
    return;
  }

  setState(STATE_CONNECTING);
  scanForDevice();
}

// runs over and over again forever
void loop() {
  if (state == STATE_WAITING) {
    Serial.println("Success! (waiting forever)");
    delay(1000 * 60 * 15);
    return;
  }
  if (state == STATE_ERROR) {
    Serial.println("Unrecoverable error. Look up.");
    delay(1000 * 60 * 60);
    return;
  }

  // Any device available from scan?
  BLEDevice device = BLE.available();
  if (!device) {
    noDevice();
    return;
  }

  // Got a device
  Serial.println();
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
  
  // Restart scan
  // scanForDevice();
}
