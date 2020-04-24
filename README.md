# NTRIP_LAN_Server_for_Arduino

An NTRIP Server suitable for use with an Arduino Mega 2560 and an Ethernet shield, to act as an NTRIP Server to a caster such as www.rtk2go.com.

This was originally modified from https://github.com/GLAY-AK2/NTRIP-server-for-Arduino

## Hardware

The intended hardware is an Arduino Mega 2560, and Ethernet Shield V2, and a SimpleRTK2B stacked on top of each other.

It may be possible to use an Arduino Uno if UART1 from the SimpleRTK2B is used, however this is currently untested.

**Note that some insulation is required** (be that some tape or an arduino shield plastic base) to stop the ethernet and GPS shields shorting between solder joints.

<img src="https://github.com/MattWoodhead/NTRIP_LAN_Client_for_Arduino/blob/master/mega_stacked.png" height="300"><img src="https://github.com/MattWoodhead/NTRIP_LAN_Client_for_Arduino/blob/master/simplertk2b_uart_to_mega.PNG" height="300">

## Getting started

1. Clone or Fork this repository.
2. Save a copy of "NTRIPConfig.h.example" and remove the ".example" suffix. Edit this new file with your mountpoint details.
3. Set UART2 of your SimpleRTK2b board to 115200 baud
4. Open "NTRIP_LAN_Client_for_Arduino.ino"
5. Setup board and ports in the Arduino Editor
6. Compile and Upload :)
