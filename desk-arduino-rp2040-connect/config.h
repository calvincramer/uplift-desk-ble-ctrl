
#ifndef CONFIG_H
#define CONFIG_H

// UUID of your bluetooth adapter.
const arduino::String ADAPTER_UUID = "000000ff-0000-1000-8000-00805f9b34fb";

// Name of your bluetooth adapter.
const arduino::String ADAPTER_NAME = "BLE Device 88DF2C";

// Bluetooth address of your bluetooth adapter.
const arduino::String ADAPTER_BT_ADDRESS = "f8:6d:c8:88:df:2c";

// Characteristic UUID for the main desk control. This changes depending on adapter revision.
// Whole UUID normally looks like "0000xxxx-0000-1000-8000-00805f9b34fb". Arduino only uses the 4 changing hex digits.
const char* CHAR_UUID_DESK_CTRL = "ff01";

// Time zone represented as offset in seconds. For example UTC-7 is -7 hours, which is -1*60*60*7 = -25,200.
const int TIME_ZONE_SHIFT_SECONDS = -1*60*60*5;

// Hour of the day to run the desk command. 0 to 23
const int HOUR_TO_RUN_DESK_COMMAND = 5;

#endif