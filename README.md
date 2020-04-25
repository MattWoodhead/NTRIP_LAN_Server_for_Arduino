# NTRIP_LAN_Server_for_Arduino

An NTRIP Server suitable for use with an Arduino Mega 2560 and an Ethernet shield, to act as an NTRIP Server to a caster such as www.rtk2go.com.

This was originally modified from https://github.com/GLAY-AK2/NTRIP-server-for-Arduino

## Hardware

1. Arduino Mega 2560 (clone or genuine - [Better quality clone tested](https://www.amazon.co.uk/gp/product/B06XKMZ3T9)).
2. Arduino Ethernet Shield V2 ([Genuine tested](https://uk.rs-online.com/web/p/processor-microcontroller-development-kits/8732285))
3. Ardusimple [SimpleRTK2B](https://www.ardusimple.com/simplertk2b/) with [Stacking Headers](https://shop.pimoroni.com/products/arduino-stackable-header-6-pin)
4. LAN Network Connection
5. Decent 5V power supply ([5V 3A barrel jack tested](https://uk.rs-online.com/web/p/ac-dc-adapters/1753307))

It may be possible to use an Arduino Uno if UART1 from the SimpleRTK2B is used, however this is currently untested.

**Note that some insulation is required** (be that some tape or an arduino shield plastic base) to stop the ethernet and GPS shields shorting between solder joints.

<p align="center">
<img src="https://github.com/MattWoodhead/NTRIP_LAN_Client_for_Arduino/blob/master/mega_stacked.png" height="400">
</p>

## Getting started

### Software
1. Clone or Fork this repository.
2. Save a copy of "NTRIPConfig.h.example" and remove the ".example" suffix. Edit this new file with your mountpoint details.
3. Set UART2 of your SimpleRTK2b board to 115200 baud
4. Open "NTRIP_LAN_Client_for_Arduino.ino"
5. Setup Mega 2560 board and ports in the Arduino Editor
6. Compile and Upload the program to the Mega 2560.

### Hardware
1. Stack the ethernet shield on top of the Mega 2560.
2. Connect the Mega 2560 to an internet connected Ethernet connection, and to your PC via USB.
3. Open the serial monitor, reset the Arduino and verify that it connects to the NTRIP caster correctly
4. Add your simpleRTK2b to the top of the Ethernet shield (**Ensure you have insulation between the shields!**)
5. Verify your DC power supply is 5V, and connect to the DC barrel jack of the Mega 2560.
6. Add a connection between the "VIN" and "5V" pins. This is easiest to do with a jumper cable in the topmost headers as shown in the photo below.
7. Add a connection between RX2 on the SimpleRTK2B and TX1 (Pin 19) on the Mega 2560.
8. Disconnect the USB cable to your PC.

<p align="center">
<img src="https://github.com/MattWoodhead/NTRIP_LAN_Client_for_Arduino/blob/master/5V_Power_Bridging.PNG" height="400">        <img src="https://github.com/MattWoodhead/NTRIP_LAN_Client_for_Arduino/blob/master/simplertk2b_uart_to_mega.PNG" height="400">
</p>
