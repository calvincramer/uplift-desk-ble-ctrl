# Uplift Desk Bluetooth Control
Control uplift desk through bluetooth adapter. The python script is intended to run on a desktop. There is also an Arduino sketch that runs a specific desk command each 24 hours at a specified time. I used it to raise the desk early in the morning so it starts standing for me.

## Python desk.py
Connect to uplift desk bluetooth adapter and send commands to move to presets, move up and down.

Based off of code from:
- https://github.com/Bennett-Wendorf/uplift-desk-controller
- https://github.com/librick/uplift-ble

This code looks for a particular revision of the bluetooth adapter. UUIDs for different revisions will need to be changed to support different revisions.

Recommended to use conda:
```sh
conda create --name desk-ctrl python=3.13
conda activate desk-ctrl
python3 -m pip install -r requirements.txt

./desk.py
```

## Arduino
This sketch runs a particular command on the desk at a certain time of day, once a day. It is intended to run on an Arduino Nano RP2040 Connect, and has code and logic specifically for this board (it has quirks!). It uses bluetooth and wifi. Wifi is used to get the time from an NTP server.

Provide your wifi credentials in a file called `config-private.h` under the `desk-arduino-rp2040-connect` folder. Here's an example:

```h
#ifndef CONFIG_PRIVATE_H
#define CONFIG_PRIVATE_H

// WiFi name and password, to get current time from NTP
char ssid[] = "name"; // your network SSID (name)
char pass[] = "password"; // your network password (use for WPA, or use as key for WEP)

#endif
```

See `config.h` for most of the configurable options. Edit `desk.h` to change the desk command that is sent to the adapter.
