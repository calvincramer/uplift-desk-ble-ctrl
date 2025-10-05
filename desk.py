#!/usr/bin/env python3

from __future__ import annotations

import asyncio

from bleak import BleakScanner, BleakClient
from bleak.backends.device import BLEDevice
from bleak import BleakScanner, BleakClient
from bleak.uuids import normalize_uuid_16
from bleak.backends.device import BLEDevice

# UUID from `bluetoothctl info <device>`
# Four hex bytes are the xxxx from: 0000xxxx-0000-1000-8000-00805f9b34fb
# Possible values for different versions of the bluetooth adapter: 0xff12, 0xfe60, 0x00ff
UUID_PRIMARY_SERVICE_DISCOVERY = normalize_uuid_16(0x00FF)

# Bluetooth characteristic UUIDs. These change depending on the version of the bluetooth adapter
CHAR_UUID_DESK_CTRL = normalize_uuid_16(0xFF01)
CHAR_UUID_DESK_HEIGHT = normalize_uuid_16(0xFF02)  # unused, I don't care about height.

# Command packets. Have the following format:
# [0xF1, 0xF1, opcode, length, payload..., checksum, 0x7E]
CMD_PACKET_WAKE = [0xF1, 0xF1, 0x00, 0x00, 0x00, 0x7E]  # The content of this command actually doesn't seem to matter, the desk just needs to wake up
CMD_PACKET_UP = [0xF1, 0xF1, 0x01, 0x00, 0x01, 0x7E]
CMD_PACKET_DOWN = [0xF1, 0xF1, 0x02, 0x00, 0x02, 0x7E]
CMD_PACKET_GO_TO_PRESET_1 = [0xF1, 0xF1, 0x05, 0x00, 0x05, 0x7E]
CMD_PACKET_GO_TO_PRESET_2 = [0xF1, 0xF1, 0x06, 0x00, 0x06, 0x7E]
CMD_PACKET_STOP = [0xF1, 0xF1, 0x2B, 0x00, 0x2B, 0x7E]
# _status_uuid = [0xF1, 0xF1, 0x07, 0x00, 0x07, 0x7E]  # unused. For height


async def cmd_preset_1(bleak_client: BleakClient) -> None:
    await cmd_wake(bleak_client)
    await bleak_client.write_gatt_char(CHAR_UUID_DESK_CTRL, CMD_PACKET_GO_TO_PRESET_1, False)
    return None


async def cmd_preset_2(bleak_client: BleakClient) -> None:
    await cmd_wake(bleak_client)
    await bleak_client.write_gatt_char(CHAR_UUID_DESK_CTRL, CMD_PACKET_GO_TO_PRESET_2, False)
    return None


async def cmd_raise(bleak_client: BleakClient) -> None:
    await cmd_wake(bleak_client)
    await bleak_client.write_gatt_char(CHAR_UUID_DESK_CTRL, CMD_PACKET_UP, False)
    return None


async def cmd_lower(bleak_client: BleakClient) -> None:
    await cmd_wake(bleak_client)
    await bleak_client.write_gatt_char(CHAR_UUID_DESK_CTRL, CMD_PACKET_DOWN, False)
    return None


async def cmd_stop_moving(bleak_client: BleakClient) -> None:
    await cmd_wake(bleak_client)
    await bleak_client.write_gatt_char(CHAR_UUID_DESK_CTRL, CMD_PACKET_STOP, False)
    return None


async def cmd_wake(bleak_client: BleakClient) -> None:
    await bleak_client.write_gatt_char(CHAR_UUID_DESK_CTRL, CMD_PACKET_WAKE, False)
    return None


async def discover_desk_ble_adapter() -> BLEDevice:
    """
    Find a desk bluetooth adapter. Stop after finding one.
    To find all the adapters if you have multiple desks, using something like:
    BleakScanner().discover(timeout=10, service_uuids=[UUID_PRIMARY_SERVICE_DISCOVERY])
    or just skip the scan and use a hardcoded device address
    """
    stop_event = asyncio.Event()
    dev: None  # bleak.backends.device.BLEDevice

    def callback(device, advertising_data):
        nonlocal dev
        dev = device
        print(dev, type(dev))
        stop_event.set()

    async with BleakScanner(callback, service_uuids=[UUID_PRIMARY_SERVICE_DISCOVERY]) as _:
        await stop_event.wait()
    return dev


async def dump_client_info(bleak_client: BleakClient) -> None:
    def get_gatt_service_string(service) -> str:
        return f"{service.uuid} - {service.description}"

    async def get_gatt_characteristic_string(characteristic, bleak_client: BleakClient) -> str:
        value: str = await bleak_client.read_gatt_char(characteristic.uuid)
        return f"{characteristic.uuid} - {characteristic.description} - ({', '.join(characteristic.properties)}): {value}"

    def get_gatt_descriptor_string(descriptor) -> str:
        return f"{descriptor.uuid} - {descriptor.description}"

    print(f"Address: {bleak_client.address}")
    print("Services:")
    for service in bleak_client.services.services.values():
        print(f"\t{get_gatt_service_string(service)}")
        for characteristic in service.characteristics:
            print(f"\t\t{await get_gatt_characteristic_string(characteristic, bleak_client)}")
            for descriptor in characteristic.descriptors:
                print(f"\t\t\t{get_gatt_descriptor_string(descriptor)}")
    return None


def print_command_options() -> None:
    print("Commands:")
    print("\th - print this help message")
    print("\t1 - move to preset 1")
    print("\t2 - move to preset 2")
    print("\tr - raise a little")
    print("\tl - lower a little")
    print("\ts - stop moving")
    print("\te - exit")
    print("\tv - dump debug info")
    return None


async def main() -> None:
    print("Scanning for desk bluetooth adapter...")
    ble_adapter = await discover_desk_ble_adapter()
    if ble_adapter is None:
        print("No desk bluetooth adapters found")
        exit(1)
    print(f"Connecting to {ble_adapter.name} - {ble_adapter.address}...")

    async with BleakClient(ble_adapter) as bc:
        print(f"Connected to {ble_adapter.name} - {bc.address}")
        print("Enter a command. 'h' for help")
        while True:
            user_command = input("> ").strip()
            print(f"{user_command} - {type(user_command)}")
            match user_command:
                case "h":
                    print_command_options()
                case "1":
                    await cmd_preset_1(bc)
                case "2":
                    await cmd_preset_2(bc)
                case "r":
                    await cmd_raise(bc)
                case "l":
                    await cmd_lower(bc)
                case "s":
                    await cmd_stop_moving(bc)
                case "e":
                    break
                case "v":
                    await dump_client_info(bc)
                case _:
                    print(f"Unknown command: {user_command}")
                    print_command_options()
    return None


if __name__ == "__main__":
    asyncio.run(main())
