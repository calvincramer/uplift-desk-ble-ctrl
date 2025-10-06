/*
  Uplift Desk Controll over Bluetooth

  The sketch is intended to run on the Arduino Nano RP2040 Connect

  TODO: raise at certain time: https://docs.arduino.cc/tutorials/communication/wifi-nina-examples/
  TODO: use LED for status
*/

#include <WiFiNINA.h>
#include <ArduinoBLE.h>
#include <WiFiUdp.h>

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

const int NUM_FAIL_SCAN_RETRY = 100;

const int STATE_INIT = 0;       // setup, before starting to try to connect to device
const int STATE_CONNECTING = 1; // attempting to find and connect to desk bluetooth adapter
const int STATE_CONNECTED = 2;  // connected, sending commands
const int STATE_WAITING = 3;    // waiting for the right time to send command
const int STATE_ERROR = 99;     // any unrecoverable error state

const unsigned long MILLISECONDS_PER_DAY = 1000 * 60 * 60 * 24;

// Networking
const unsigned int localPort = 2390;         // local port to listen for UDP packets
const IPAddress timeServer(129, 6, 15, 28);  // time.nist.gov NTP server
const int NTP_PACKET_SIZE = 48;              // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE];         // buffer to hold incoming and outgoing packets
WiFiUDP Udp;                                 // A UDP instance to let us send and receive packets over UDP
char ssid[] = "TODO";                        // your network SSID (name)
char pass[] = "TODO";                        // your network password (use for WPA, or use as key for WEP)

int state = STATE_INIT;  // Overall state
int scanFailCount = 0;   // number of times in main loop not find the right device
// unix time (seconds) from NTP
unsigned long unix_time = 0;
unsigned long ms_got_unix_time = 0;


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
  // Do not delay long here. I had delay of 1 second here and the scan fails to consistently find the device.
  delay(10);
  scanFailCount += 1;
  if (scanFailCount >= NUM_FAIL_SCAN_RETRY) {
    Serial.println("Restarting scan");
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

  // Initialize bluetooth
  Serial.println("Starting BLE library");
  if (!BLE.begin()) {
    setState(STATE_ERROR);
    Serial.println("Failed to start BLE library");
    return;
  }

  // Initialize WiFi
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    setState(STATE_ERROR);
  }
  if (WiFi.firmwareVersion() < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the wifi firmware");
  }

  setState(STATE_WAITING);
}

// runs over and over again forever
void loop() {
  if (state == STATE_ERROR) {
    Serial.println("Unrecoverable error. Look up.");
    delay(1000 * 60 * 60);
    return;
  }

  if (state == STATE_WAITING) {
    // Get current time
    get_time();

    // TODO: convert unix time to current hour in hard-coded time zone
    // TODO: if time is 5am then move to STATE_CONNECTING
    // TODO: if time is 10am then reset waiting for 5am

    Serial.println("(waiting)");
    delay(1000 * 60 * 15);

    // TODO after get to right time:
    // setState(STATE_CONNECTING);
    // scanForDevice();
    return;
  }

  // Try to connect and control device:
  
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
  Serial.print("Skip device control...")
  // controlDesk(device);
  
  // Restart scan
  // scanForDevice();
}

void get_time() {
  // Expire saved time every once in a while
  if ((millis() - ms_got_unix_time) > MILLISECONDS_PER_DAY) {
    unix_time = 0;
    ms_got_unix_time = 0;
  }

  // Don't have time yet
  if (unix_time == 0) {
    get_time_from_ntp();
  }
}

void get_time_from_ntp() {
  // TODO:    if not connect to wifi then connect
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();
  Serial.println("\nStarting connection to server...");
  Udp.begin(localPort);

  // TODO:    get it from NTP
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  if (Udp.parsePacket()) {
    Serial.println("packet received");
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = ");
    Serial.println(secsSince1900);
    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);
    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
  }

  ms_got_unix_time = millis();
}

unsigned long sendNTPpacket(IPAddress& address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();

}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
