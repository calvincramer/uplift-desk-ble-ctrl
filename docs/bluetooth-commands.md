# Bluetooth Commands
Find device:
```sh
bluetoothctl scan on
bluetoothctl devices
# continue until find this device:
# Device F8:6D:C8:88:DF:2C BLE Device 88DF2C
bluetoothctl scan off
```

Connect and get info:
```sh
bluetoothctl info F8:6D:C8:88:DF:2C
# Device F8:6D:C8:88:DF:2C (public)
#         Name: BLE Device 88DF2C
#         Alias: BLE Device 88DF2C
#         Paired: no
#         Bonded: no
#         Trusted: no
#         Blocked: no
#         Connected: no
#         LegacyPairing: no
#         UUID: Unknown                   (000000ff-0000-1000-8000-00805f9b34fb)
#         RSSI: 0xffffffa2 (-94)

bluetoothctl pair F8:6D:C8:88:DF:2C

Attempting to pair with F8:6D:C8:88:DF:2C
[CHG] Device F8:6D:C8:88:DF:2C Connected: yes
[NEW] Primary Service (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service000a
        00001801-0000-1000-8000-00805f9b34fb
        Generic Attribute Profile
[NEW] Primary Service (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service000b
        0000ff00-0000-1000-8000-00805f9b34fb
        Unknown
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service000b/char000c
        0000ff01-0000-1000-8000-00805f9b34fb
        Unknown
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service000b/char000e
        0000ff02-0000-1000-8000-00805f9b34fb
        Unknown
[NEW] Descriptor (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service000b/char000e/desc0010
        00002902-0000-1000-8000-00805f9b34fb
        Client Characteristic Configuration
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service000b/char0011
        0000fe63-0000-1000-8000-00805f9b34fb
        Connected Yard, Inc. 
[NEW] Descriptor (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service000b/char0011/desc0013
        00002902-0000-1000-8000-00805f9b34fb
        Client Characteristic Configuration
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service000b/char0014
        0000fe64-0000-1000-8000-00805f9b34fb
        Siemens AG
[NEW] Descriptor (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service000b/char0014/desc0016
        00002902-0000-1000-8000-00805f9b34fb
        Client Characteristic Configuration
[NEW] Primary Service (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service0017
        0000180a-0000-1000-8000-00805f9b34fb
        Device Information
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service0017/char0018
        00002a29-0000-1000-8000-00805f9b34fb
        Manufacturer Name String
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service0017/char001a
        00002a24-0000-1000-8000-00805f9b34fb
        Model Number String
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service0017/char001c
        00002a25-0000-1000-8000-00805f9b34fb
        Serial Number String
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service0017/char001e
        00002a27-0000-1000-8000-00805f9b34fb
        Hardware Revision String
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service0017/char0020
        00002a26-0000-1000-8000-00805f9b34fb
        Firmware Revision String
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service0017/char0022
        00002a28-0000-1000-8000-00805f9b34fb
        Software Revision String
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service0017/char0024
        00002a23-0000-1000-8000-00805f9b34fb
        System ID
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service0017/char0026
        00002a2a-0000-1000-8000-00805f9b34fb
        IEEE 11073-20601 Regulatory Cert. Data List
[NEW] Characteristic (Handle 0x0000)
        /org/bluez/hci0/dev_F8_6D_C8_88_DF_2C/service0017/char0028
        00002a50-0000-1000-8000-00805f9b34fb
        PnP ID
[CHG] Device F8:6D:C8:88:DF:2C UUIDs: 000000ff-0000-1000-8000-00805f9b34fb
[CHG] Device F8:6D:C8:88:DF:2C UUIDs: 00001800-0000-1000-8000-00805f9b34fb
[CHG] Device F8:6D:C8:88:DF:2C UUIDs: 00001801-0000-1000-8000-00805f9b34fb
[CHG] Device F8:6D:C8:88:DF:2C UUIDs: 0000180a-0000-1000-8000-00805f9b34fb
[CHG] Device F8:6D:C8:88:DF:2C UUIDs: 0000ff00-0000-1000-8000-00805f9b34fb
[CHG] Device F8:6D:C8:88:DF:2C ServicesResolved: yes
[CHG] Device F8:6D:C8:88:DF:2C Modalias: bluetooth:v2717p3200d0110
[CHG] Device F8:6D:C8:88:DF:2C ServicesResolved: no
[CHG] Device F8:6D:C8:88:DF:2C Connected: no
Failed to pair: org.bluez.Error.AuthenticationCanceled
# then bluetoothctl segfaulted
```

## Get More Info:
```sh
gatttool -b F8:6D:C8:88:DF:2C -I
connect
primary
characteristics 0x0017 0xffff
char-read-hnd 18   # 02 19 00 29 2a -> gibberish ASCII
char-read-hnd 1a   # 02 1b 00 24 2a -> gibberish ASCII
char-read-hnd 1c   # 02 1d 00 25 2a -> gibberish ASCII
char-read-hnd 1e   # 02 1f 00 27 2a -> gibberish ASCII
char-read-hnd 20   # 02 21 00 26 2a -> gibberish ASCII
char-read-hnd 22   # 02 23 00 28 2a -> gibberish ASCII
char-read-hnd 24   # 02 25 00 23 2a -> gibberish ASCII
char-read-hnd 26   # 02 27 00 2a 2a -> gibberish ASCII
char-read-hnd 28   # 02 29 00 50 2a -> gibberish ASCII
```
