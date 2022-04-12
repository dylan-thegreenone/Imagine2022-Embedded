# CSHacked—Embedded Team

This repo contains all source code and information pertaining to the configuration and operation of the embedded portion of CSHacked. It's basically nothing right now. It just scans the bluetooth airwaves and lists them onscreen. That somehow needs to be put into a database...

Below, you'll find some (slightly modified) documentation from the repo this project was pulled from.

| Supported Targets | ESP32 |
| ----------------- | ----- |

This project is based off of the [ESP-IDF BT-INQUIRY demo](https://github.com/espressif/esp-idf/tree/v4.3.2/examples/bluetooth/bluedroid/classic_bt/bt_discovery)

Demo of Classic Bluetooth Device and Service Discovery. Performs inquiry to search for devices, relays names, MAC addresses, and RSSI to the screen.

## How to use

Add a unique device number with
```
idf.py menuconfig
```
Navigate to section labeled Main and set a UNIQUE device number
Flash code onto ESP as outlined in HACKING/README.md

### Hardware Required

This example is designed to run on commonly available ESP32 development board, e.g. ESP32-DevKitC.

### Configure the project

```
idf.py menuconfig
```

### Build and Flash

Build the project and flash it to the board, then run the monitor tool to view serial output:

```
idf.py -p PORT flash monitor
```

(Replace PORT with the name of the serial port to use. For example, /dev/ttyUSB0)

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

## Troubleshooting

- Some old BT devices don't place their device name in EIR. Users can obtain the peer device name in `bt_app_gap_cb` `ESP_BT_GAP_DISC_RES_EVT` event handler or resolve it in EIR as in the function `get_name_from_eir`.

- The ESP32 places its device name in EIR by default.
