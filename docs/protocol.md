# Uplift / Lierda Bluetooth Protocol
Service UUID types (aka device UUID)
- 0xfe60 `0000fe60-0000-1000-8000-00805f9b34fb`
- 0xff12 `0000ff12-0000-1000-8000-00805f9b34fb`
- 0x00ff --> mine (`000000ff-0000-1000-8000-00805f9b34fb`). This document covers 00ff.

## Services, Characteristics, Descriptors

Device service UUID 000000ff-0000-1000-8000-00805f9b34fb
- service000a 00001801-0000-1000-8000-00805f9b34fb Generic Attribute Profile
- **service000b** 0000ff00-0000-1000-8000-00805f9b34fb Unknown
    - **char000c** 0000ff01-0000-1000-8000-00805f9b34fb Unkown **(this.OutCharacteristic - control)**
    - **char000e** 0000ff02-0000-1000-8000-00805f9b34fb Unknown **(this.InCharacteristic - height)**
        - desc0010 00002902-0000-1000-8000-00805f9b34fb Client Characteristic Configuration
    - **char0011** 0000fe63-0000-1000-8000-00805f9b34fb Connected Yard, Inc. **(this.BTnameCharacteristic)**
        - desc0013 00002902-0000-1000-8000-00805f9b34fb Client Characteristic Configuration
    - **char0014** 0000fe64-0000-1000-8000-00805f9b34fb Siemens AG
        - desc0016 00002902-0000-1000-8000-00805f9b34fb Client Characteristic Configuration
- service0017 0000180a-0000-1000-8000-00805f9b34fb **Device Information**
    - char0018 00002a29-0000-1000-8000-00805f9b34fb Manufacturer Name String
    - char001a 00002a24-0000-1000-8000-00805f9b34fb Model Number String
    - char001c 00002a25-0000-1000-8000-00805f9b34fb Serial Number String
    - char001e 00002a27-0000-1000-8000-00805f9b34fb Hardware Revision String
    - char0020 00002a26-0000-1000-8000-00805f9b34fb Firmware Revision String
    - char0022 00002a28-0000-1000-8000-00805f9b34fb Software Revision String
    - char0024 00002a23-0000-1000-8000-00805f9b34fb System ID
    - char0026 00002a2a-0000-1000-8000-00805f9b34fb IEEE 11073-20601 Regulatory Cert. Data List
    - char0028 00002a50-0000-1000-8000-00805f9b34fb PnP ID

## Desk Control "OutCharacteristic" Commands

All of these are Java byte[] arrays

- **DESK UP**: {0xF1, 0xF1, 0x01, 0x00, 0x01, 0x7E}
- **DESK DOWN**: {0xF1, 0xF1, 0x02, 0x00, 0x02, 0x7E}
- **MOVE TO PRESET 1**: {0xF1, 0xF1, 0x05, 0x00, 0x05, 0x7E}
- **MOVE TO PRESET 2**: {0xF1, 0xF1, 0x06, 0x00, 0x06, 0x7E}
- **STOP MOVEMENT**: {0xF1, 0xF1, 0x2B, 0x00, 0x2B, 0x7E}
- **REQUEST HEIGHT LIMITS**: {0xF1, 0xF1, 0x07, 0x00, 0x07, 0x7E} -> maybe current height
- **MOVE TO SPECIFIED HEIGHT**: {0xF1, 0xF1, 0x1B, 0x02, bArrIntToBytes[2], bArrIntToBytes[3], (byte) (bArrIntToBytes[2] + 29 + bArrIntToBytes[3]), 0x7E}
- {0xF1, 0xF1, 0xFE, 0x00, 0xFE, 0x7E}
- **SET CALIBRATION OFFSET**: {0xF1, 0xF1, 0x10, 0x02, CalibrateActivity.this.a2[2], CalibrateActivity.this.a2[3], (byte) (CalibrateActivity.this.a2[2] + 18 + CalibrateActivity.this.a2[3]), 0x7E}
- {0xF1, 0xF1, 0x0C, 0x00, 0x0C, 0x7E}
- {0xF1, 0xF1, 0x19, 0x01, 0x00, 26, 0x7E}
- {0xF1, 0xF1, 0x19, 0x01, 0x01, 27, 0x7E}
- {0xF1, 0xF1, 0x1F, 0x01, 0x00, 0x20, 0x7E}
- {0xF1, 0xF1, 0x20, 0x00, 0x20, 0x7E}

---

"Command packet": [0xF1, 0xF1, 0xOPCODE, 0xPAYLOAD_LENGTH, 0xPAYLOAD..., 0xCHECKSUM(OPECODE, PAYLOAD), 0x7E]
