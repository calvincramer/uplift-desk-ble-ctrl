import asyncio
import builtins
import unittest
from unittest import mock

from bleak.exc import BleakError

import desk


class FakeClient:
    """Stand-in for BleakClient whose connection drops before the first write."""

    def __init__(self, *args, **kwargs):
        self.address = "AA:BB:CC:DD:EE:FF"
        self.is_connected = True
        self.connect_calls = 0
        self.writes = []
        self.fail_next_write = True

    async def __aenter__(self):
        return self

    async def __aexit__(self, *exc):
        return False

    async def connect(self):
        self.connect_calls += 1
        self.is_connected = True

    async def write_gatt_char(self, uuid, data, response):
        if self.fail_next_write:
            self.fail_next_write = False
            self.is_connected = False
            raise BleakError("Not connected")
        self.writes.append(list(data))


class MainLoopTest(unittest.TestCase):
    def run_main(self, inputs):
        client = FakeClient()
        device = mock.Mock(name="dev", address=client.address)
        device.name = "BLE Device"
        with (
            mock.patch.object(desk, "discover_desk_ble_adapter", mock.AsyncMock(return_value=device)),
            mock.patch.object(desk, "BleakClient", return_value=client),
            mock.patch.object(builtins, "input", side_effect=inputs),
            mock.patch("builtins.print"),
        ):
            asyncio.run(desk.main())
        return client

    def test_command_after_disconnect_does_not_crash_and_reconnects(self):
        client = self.run_main(["2", "2", "e"])
        self.assertEqual(client.connect_calls, 1)
        self.assertIn(desk.CMD_PACKET_GO_TO_PRESET_2, client.writes)


if __name__ == "__main__":
    unittest.main()
