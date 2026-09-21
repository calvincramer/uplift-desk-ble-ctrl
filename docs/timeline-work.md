## 0. Install for later

Steps: install UPX
1. https://github.com/upx/upx/releases/download/v5.0.2/upx-5.0.2-amd64_linux.tar.xz
2. Extract

## 1. uplift-ble
- https://github.com/librick/uplift-ble

Follow:
```sh
cd <somewhere>
git clone git@github.com:librick/uplift-ble.git
cd uplift-ble

conda create --name uplift-ble python=3.12
conda activate uplift-ble
python3 -m pip install -e '.[cli,dev]'
python3 -m pytest -v
python3 scripts/uplift_ble_cli.py --help

python3 -m pip install pyinstaller
# 0
pyinstaller scripts/uplift_ble_cli.py
# 1
pyinstaller scripts/uplift_ble_cli.py --strip
# 2
pyinstaller scripts/uplift_ble_cli.py --strip --upx-dir=/home/cal/repos/DESK/upx-5.0.2-amd64_linux/upx
# 3
pyinstaller scripts/uplift_ble_cli.py --strip --upx-dir=/home/cal/repos/DESK/upx-5.0.2-amd64_linux/upx --onefile
```

Pyinstaller
- 0: 5566832, shared
- 1: 5566832, shared
- 2: 5566832, shared
- 3: **13170456**, shared

Pretty big, let's try something else

## 2. uplift-ble-helper
- https://github.com/mdisibio/uplift-ble-helper

Steps:
1. clone
2. cd clone
3. install golang
4. `go build`

5. Edit files to build on linux

```sh
go build   # 16 MiB 16153430
go build -ldflags "-w -s" # 11 MiB 11075876
go build -ldflags "-w -s" && upx -9 upliftdesk # 3.6 MiB 3704684
go build -ldflags "-w -s" && upx --best upliftdesk  # 3.6 MiB 3702656
go build -ldflags "-w -s" && upx --best --ultra-brute upliftdesk # 3.0 MiB 3127564
go build -ldflags "-w -s" && upx --best --brute upliftdesk # 3.0 MiB 3141084
```

Little big, I know I don't need prometheus

## 3. uplift-ble-helper-small
- https://github.com/calvincramer/uplift-ble-helper-small
1. My repo, clone

```sh
go build -ldflags "-w -s" && upx --best --brute upliftdesk # 1.1 MiB 1126184
```

## 4. Run on Desktop

### 4.1 Try uplift-ble-helper-small
1. Plug in uplift bluetooth adapter
```sh
cd /home/cal/repos/DESK/uplift-ble-helper-small
go build -ldflags "-w -s" && upx --best --brute upliftdesk

./upliftdesk -scan
```
Doesn't work for uplift-ble-helper finding bluetooth device

### 4.2 Try uplift-ble
```sh
cd /home/cal/repos/DESK/uplift-ble
conda activate uplift-ble

python3 scripts/uplift_ble_cli.py discover
```
Fail

## 5. Sanity Check
Run `bluetoothctl devices`, find the device is:
`Device F8:6D:C8:88:DF:2C BLE Device 88DF2C`

None of the uplift desk controller repos are working.
I figured out it's the bluetooth service UUID used to match uplift desk bluetooth adapter devices.
**Mine** is not 0xFF12, it's not 0xFE60. But it is 0x00FF which I found from an issue on github: https://github.com/Bennett-Wendorf/hass-uplift-desk/issues/4
UUIDs:
- `0000ff12-0000-1000-8000-00805f9b34fb` not mine
- `0000fe60-0000-1000-8000-00805f9b34fb` not mine
- `000000ff-0000-1000-8000-00805f9b34fb` mine

## 6. Run on Desktop with UUID fix
### 6.1 uplift-ble
```sh
cd /home/cal/repos/DESK/uplift-ble
conda activate uplift-ble

python3 scripts/uplift_ble_cli.py discover
# "address": "F8:6D:C8:88:DF:2C",
# "name": "BLE Device 88DF2C",
# "advertised_service_uuids": [
#     "000000ff-0000-1000-8000-00805f9b34fb"
# ]

python3 scripts/uplift_ble_cli.py --address 'F8:6D:C8:88:DF:2C' get-current-height
# Got an error:
# BleakCharacteristicNotFoundError: Characteristic 0000fe62-0000-1000-8000-00805f9b34fb was not found!
```

### 6.2 uplift-ble-helper-small
```sh
./upliftdesk -scan
./upliftdesk -device 'F8:6D:C8:88:DF:2C'
# fails
```

### 6.3 uplift-desk-controller-dump-info
- follow https://github.com/Bennett-Wendorf/hass-uplift-desk/issues/4#issuecomment-3006290880
- clone repo, checkout to v1.2.0
```sh
cd /home/cal/repos/DESK/uplift-desk-controller-dump-info
conda create --name uplift-desk-controller python=3.13
conda activate uplift-desk-controller
conda install conda-forge::pipenv
pipenv lock
pipenv requirements > requirements.txt
pip install -r requirements.txt

pipenv run python demo.py
    v
```

Get this output:
```
Services:
        0000180a-0000-1000-8000-00805f9b34fb - Device Information
                00002a25-0000-1000-8000-00805f9b34fb - Serial Number String - (read): bytearray(b'Serial Number')
                00002a29-0000-1000-8000-00805f9b34fb - Manufacturer Name String - (read): bytearray(b'Manufacturer Name')
                00002a50-0000-1000-8000-00805f9b34fb - PnP ID - (read): bytearray(b"\x01\x17\'\x002\x10\x01")
                00002a24-0000-1000-8000-00805f9b34fb - Model Number String - (read): bytearray(b'L-BTMEB95-07014-03')
                00002a2a-0000-1000-8000-00805f9b34fb - IEEE 11073-20601 Regulatory Cert. Data List - (read): bytearray(b'\xfe\x00experimental')
                00002a23-0000-1000-8000-00805f9b34fb - System ID - (read): bytearray(b',\xdf\x88\x00\x00\xc8m\xf8')
                00002a27-0000-1000-8000-00805f9b34fb - Hardware Revision String - (read): bytearray(b'Hardware Revision')
                00002a28-0000-1000-8000-00805f9b34fb - Software Revision String - (read): bytearray(b'v1.01.Jul 25 2023')
                00002a26-0000-1000-8000-00805f9b34fb - Firmware Revision String - (read): bytearray(b'Rev01')
        0000ff00-0000-1000-8000-00805f9b34fb - Vendor specific
                0000fe63-0000-1000-8000-00805f9b34fb - Connected Yard: Inc. - (write-without-response, write, notify): bytearray(b'')
                        00002902-0000-1000-8000-00805f9b34fb - Client Characteristic Configuration
                0000ff01-0000-1000-8000-00805f9b34fb - Vendor specific - (write-without-response, write): bytearray(b'')
                0000fe64-0000-1000-8000-00805f9b34fb - Siemens AG - (write-without-response, write, notify): bytearray(b'')
                        00002902-0000-1000-8000-00805f9b34fb - Client Characteristic Configuration
                0000ff02-0000-1000-8000-00805f9b34fb - Vendor specific - (notify): bytearray(b'')
                        00002902-0000-1000-8000-00805f9b34fb - Client Characteristic Configuration
        00001801-0000-1000-8000-00805f9b34fb - Generic Attribute Profile
```

Using this thread: https://github.com/Bennett-Wendorf/hass-uplift-desk/issues/5 look for `write-without-response` and `notify` services:
```
0000fe63-0000-1000-8000-00805f9b34fb - Connected Yard: Inc. - (write-without-response, write, notify): bytearray(b'')
0000fe64-0000-1000-8000-00805f9b34fb - Siemens AG - (write-without-response, write, notify): bytearray(b'')
```
So service UUIDS:
- "height" originally `fe61`, I'll use `fe64`
- "control" originally `fe62`, I'll use `fe63`

### 6.4 uplift-ble
1. Modify `BLE_SERVICE_UUID_UPLIFT_DISCOVERY_LIERDA_V2` to be `00ff`
2. Modify `BLE_CHAR_UUID_UPLIFT_DESK_CONTROL` to be `FE63`
3. Modify `BLE_CHAR_UUID_UPLIFT_DESK_OUTPUT` to be `FE64`
```sh
cd /home/cal/repos/DESK/uplift-ble
conda activate uplift-ble

python3 scripts/uplift_ble_cli.py discover
python3 scripts/uplift_ble_cli.py --address 'F8:6D:C8:88:DF:2C' get-current-height
python3 scripts/uplift_ble_cli.py --address 'F8:6D:C8:88:DF:2C' move-up
# didn't work
```

### 6.5 Try again with uplift-ble-helper-small and UUID fixes
Make 3 changes to UUIDs.
```sh
./upliftdesk -device 'F8:6D:C8:88:DF:2C'
```
Nope...

### 6.6 Try with desk_control repo
```sh
conda create --name desk-control python=3.12
conda activate desk-control
pip install -r requirements.txt
python3 desk_control.py raise 1
# edit desk_control.py line 63 to have my device's bluetooth address
```
Doesn't work


## 7. Actually Need to Figure Out What UUIDs and Services and Characteristics I Need, the Messages
Goal: try on android. Inspect APK, reverse engineer.
Following https://github.com/librick/uplift-ble/pull/2#issuecomment-3157223949

1. Download Uplift desk (v1.1.1) on old ANDROID phone. Verified it works.
2. Download APK: https://apkpure.com/uplift-desk-app/com.jiecang.app.android.uplifts/download
3. Run
```sh
cd ~/Downloads
mkdir foo
cp Uplift Desk App_1.0.1_APKPure.xapk foo
cd foo
unzip "Uplift Desk App_1.0.1_APKPure.xapk"
```
4. Install jadx
```sh
flatpak install flathub com.github.skylot.jadx
```
5. Run jadx
    1. `flatpak run com.github.skylot.jadx`
    2. Open file `Uplift Desk App_1.0.1_APKPure.xapk
    3. Look at source code -> com -> jiecang.app.android.aidesk -> BluetoothLeService and NewMainActivity

## 8. Am I ABSOLUTELY POSITIVE that my protocol is 00ff? Not fe60 or ff12?
YES. Run `bluetoothctl info F8:6D:C8:88:DF:2C` and get:
```
UUID: Unknown (000000ff-0000-1000-8000-00805f9b34fb)
```

## 9. Look into the bluetooth device serivces, characteristics, descriptors
See protocol.md

## 10. Look around Android APK more
- No mention of `000000ff-0000-1000-8000-00805f9b34fb`
- Present: `0000fe60-0000-1000-8000-00805f9b34fb`
- Present: `0000ff12-0000-1000-8000-00805f9b34fb`

## 11. Get Android App version 1.1.1
Google Play Store URL: https://play.google.com/store/apps/details?id=app.android.uplifts
Use APK downloader to download
I got version 1.1.0

APK info:
class BluetoothLeService
    public static final String UUID_SERVICE5 = "000000ff-0000-1000-8000-00805f9b34fb";
    public static final String UUID_CHARACTERISTIC_READ5 = "000002ff-0000-1000-8000-00805f9b34fb";
    public static final String UUID_CHARACTERISTIC_WRITE5 = "000001ff-0000-1000-8000-00805f9b34fb";
    public static final String UUID_CHARACTERISTIC_SET5 = "000036ef-0000-1000-8000-00805f9b34fb";

I have device UUID 00ff, which looks like UUID_SERVICE5, but lots of characteristics line up with "service4"
Since "serivce000b" is UUID `0000ff00-0000-1000-8000-00805f9b34fb` I think we are UUID_SERVICE4
isLierdaNotify = true
InCharacteristic = bluetoothGattCharacteristic2 (which is `0000ff02-0000-1000-8000-00805f9b34fb`)
isLierdaWrite = true
OutCharacteristic = bluetoothGattCharacteristic2 (which is `0000ff01-0000-1000-8000-00805f9b34fb`)
isLierdaBtName = true
NewMainActivity.is3 = false
BTnameCharacteristic = bluetoothGattCharacteristic2 (which is `0000fe63-0000-1000-8000-00805f9b34fb`)

I think I got the right characteristic UUIDs for "control" (InCharacteristic) and "output" (OutCharacteristic)

## 12. Modify the repos and try again
1. Tried uplift-ble, didn't work.
2. Tried uplift-desk-controller, it works!
3. Tried uplift-ble-helper, doesn't work.

My presets:
1: 47.9 -> 29.0
2: 45.5
3: 29.0 -> 47.9
4: 29.9 -> 40

uplift-desk-controller
- d move_to_sitting -> preset 1
- u move_to_standing -> preset 2

Yay, progress!

## 13. Figure out the exact messages and contents sent to device
This will be useful later when running on arduino
Figured out, see protocol.md.

## 14. Make a better version of uplift-desk-controller
Done. https://github.com/calvincramer/uplift-desk-ble-ctrl

## 15. How big is my uplift-desk-ble-ctrl compiled?
```sh
cd /home/cal/repos/DESK/cal-uplift-desk-ble-ctrl
conda activate desk-ctrl
python3 -m pip install pyinstaller
pyinstaller desk.py --strip --onefile
```
13 MiB

## 16. Run on Arduino Nano RP2040 Connect

Todo:
1. Wait for time of day 5am
2. Connect to device (hardcode address)
3. Go to preset 2 -> [0xF1, 0xF1, 0x06, 0x00, 0x06, 0x7E]
4. Repeat

Steps:
1. Install Arduino IDE, and run
2. Plugin Arduino
3. Select the Board
4. `sudo usermod -a -G dialout $USER`
5. Restart computer
6. Setup more:
```sh
cd /etc/udev/rules.d
sudo su
echo 'SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", GROUP="plugdev", MODE="0666"' > 99-arduino.rules
exit
```
7. More setup for udev:
```sh
cd /tmp
mkdir z
cd z
wget https://raw.githubusercontent.com/arduino/ArduinoCore-mbed/refs/heads/main/post_install.sh
chmod +x post_install.sh
sudo ./post_install.sh
```

I got it working one time. Scanning and connecting to the adapter is REALLY slow.
Fixed it. There was a delay while scanning for devices. Turns out calling `BLE.available()` more works better. Very weird behavior.

## 17. Arduino get current time, raise at certain time.
Doing...
Done!
All done with Arudino sketch. Just need to test it out tonight to see if it rises at 5am.

Also have an issue with WiFi breaking when attempt to control LEDs...

## 18. Verify it works overnight
First night - failed. It didn't have power. My power brick was broke. I found one that works. Now the green power LED lights up. Duh!
Second night - failed. I don't know what happened.
Third night - It works! I made code changes to make sure everything is working. Get LED status light.

## 19. Install in a nice case
All done. Needed some dremmeling.

## 20. Conclusion
Arduino Nano RP2040 Connect is full of suprises! Pretty frustrating learning that can't use Wifi and Bluetooth at the same time. Also that can't use the RGB LED the same time as Bluetooth! I shouldn't have had to read the docs top to bottom with a fine-tooth comb to figure these limitations out. These should be listed very clearly somewhere. Save some hair being pulled out!
