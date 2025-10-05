# Uplift Desk Bluetooth Control
Connect to uplift desk bluetooth adapter and send commands to move to presets, move up and down.

Based off of code from:
- https://github.com/Bennett-Wendorf/uplift-desk-controller
- https://github.com/librick/uplift-ble

This code looks for a particular revision of the bluetooth adapter. UUIDs for different revisions will need to be changed to support different revisions.

## Python
Recommended to use conda:
```sh
conda create --name desk-ctrl python=3.13
conda activate desk-ctrl
python3 -m pip install -r requirements.txt

./desk.py
```
