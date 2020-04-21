# NTRIP_LAN_Client_for_Arduino
An NTRIP Client suitable for use with an Arduino Uno and an Ethernet shield

This script and the associated files allow and Arduino Uno and an Ethernet shield to act as an NTRIP Server.

Modified from https://github.com/GLAY-AK2/NTRIP-server-for-Arduino

## Hardware

The intended hardware is an Arduino Uno, and Ethernet Shield V2, and a SimpleRTK2B stacked on top of each other.

*Note that some insulation is required* (be that some tape or an arduino shield plastic base) to stop the ethernet and GPS shields shorting between solder joints.

<img src="https://github.com/MattWoodhead/NTRIP_LAN_Client_for_Arduino/blob/master/uno_stacked.png" height="400">

## Getting started

1. Clone or Fork this repository.
2. Save a copy of "NTRIPConfig.h.example" and remove the ".example" suffix. Edit this new file with your mountpoint details.
3. Open "NTRIP_LAN_Client_for_Arduino.ino"
4. Setup board and ports in the Arduino Editor
5. Compile and Upload :)
