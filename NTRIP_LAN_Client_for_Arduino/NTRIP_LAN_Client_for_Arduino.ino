/* This script and the associated files allow and Arduino Mega 2560 and an Ethernet shield to act as an NTRIP Server.
 * See the documentation at https://github.com/MattWoodhead/NTRIP_LAN_Server_for_Arduino
 * 
 * Copyright MattWoodhead
 */

#include "NTRIPServer.h"
#include <Ethernet.h>

// This is where the caster address, port, mopuntpoint, password etc are stored.
#include "NTRIPConfig.h"                                               

#define DEBUG false

// SimpleRTK2B RX2 is connected to Mega RX1 (19)
HardwareSerial & RtcmSerial = Serial1;                              // this is assigning a name to the serial 1 port

// Randomly generated at https://www.hellion.org.uk/cgi-bin/randmac.pl 
const byte mac_address[] = {0xA8,0x61,0x0a,0xAE,0x7E,0x6C};           // Note: Newer boards come with a fixed MAC address
byte ip_address[] = {192,168,1,184};                                  // Assign an IP address if DHCP is not available

uint8_t byte_buffer[256];                                                // buffer into which we will read the RtcmSerial port
int byte_count = 0;                                                   // counter to keep track of buffer length

unsigned long previous_time = 0;                                      // timers to decide when to send 1008 message
unsigned long latest_time = 0; 
const int rtcm1008_rate = 10000;                                      // Rate at which to send 1008 message in milliseconds
bool rtcm1008_injected = false;                                       // A flag used to determine if the 1008 message has been sent successfully


const uint8_t rtcm1008[] = {                                          // This byte sends a 1008 message with ADVNULL as the antenna model
0xd3,0x00,0x14,0x3f,0x00,0x00,0x0e,0x41,0x44,0x56,0x4e,0x55,0x4c,
0x4c,0x41,0x4e,0x54,0x45,0x4e,0x4e,0x41,0x00,0x00,0x79,0x06,0x89
};
const uint8_t newline_byte = byte('\n');

NTRIPServer ntrip_s;                                                  // Create an instance of the NTRIPServer class

void setup() {
  delay(500);
  Serial.begin(115200);
  Serial.println(F("Starting..."));
  RtcmSerial.begin(115200);
  delay(250);
  
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);                                              // disable SD card
  
  delay(250);

  //Ethernet.begin(mac_address, ip_address);
  Ethernet.begin(mac_address);                                        // Use this one if using DHCP
  delay(100);
  while (Ethernet.linkStatus() == LinkOFF) {                          // Wait for ethernet to connect
    delay(500);
    Serial.print(".");
  }

  Serial.println(F("LAN connected"));
  Serial.print(F("IP: "));
  Serial.println(Ethernet.localIP());

  delay(100);
  if (!ntrip_s.subStation(host, httpPort, mountPoint, password, sourceString)) {
    delay(15000);
  }
  Serial.println(F("MountPoint Subscribed"));
  previous_time = millis();
}

void loop() {
  if (ntrip_s.connected()) {
    while (RtcmSerial.available()) {
      byte c = RtcmSerial.read();                                       // read in a byte
      byte_buffer[byte_count++] = c;                                    // add byte to buffer and increment counter
      
      if ((c == newline_byte) || (byte_count == sizeof(byte_buffer)-1)) {       // if the buffer gets full or we see the newline character
        //ntrip_s.write((uint8_t*)byte_buffer, byte_count);               // send buffer to the ntrip server
        ntrip_s.write(byte_buffer, byte_count);               // send buffer to the ntrip server
        if (DEBUG){Serial.println(F("packet sent"));}                    // indicate sent buffer for debugging only
        
        byte_count = 0;                                                 // reset counter
        for (unsigned int i=0;i<sizeof(byte_buffer);i++){               // wipe the buffer for the next loop
          byte_buffer[i] = 0x00;
        }
      }
      
      latest_time = millis();
      if ((not rtcm1008_injected) && (byte_count > 0)) {                // if the rtcm1008 flag is false (i.e. time has elapsed or message not yet injected) and there is more than zero byte in the buffer
        if ((c == 0xd3) && (byte_count + sizeof(rtcm1008) < sizeof(byte_buffer)-2)) {    // if the premable byte of a message is the latest to arrive at the serial port, and there is enough room in the buffer to inject the 1008 message
          if (DEBUG){Serial.println(F("preamble recognised"));}         // indicate preamble located for debugging only
          byte_count--;                                                 // decrement the buffer counter so we overwrite the preamble character
          for (unsigned int i=0;i<sizeof(rtcm1008);i++){                // add the 1008 message bytes to the buffer 
            byte_buffer[byte_count++] = rtcm1008[i];
          }
          byte_buffer[byte_count++] = c;                                // add the preamble byte back in
          rtcm1008_injected = true;                                     // set the sent flag to true so the timer gets reset to delay the next 1008 transmission
          previous_time = millis();                                     // reset the timer
        }
      } else {                                                          // if the rtcm1008 flag is true
        if ((latest_time - previous_time) > rtcm1008_rate){             // check if the specified length of time has passed since the last 1008 message was sent
          rtcm1008_injected = false;                                    // set the rtcm1008 flag to false so that the time is only checked once. This should present any rollover if the injection takes longer than the specified time period
        }
      }

    }
  } else {                                                              // if ntrip connection fails
    ntrip_s.stop();                                                     // close current socket connection
    byte_count = 0;                                                     // reset the counter
    for (unsigned int i=0;i<sizeof(byte_buffer);i++){                   // wipe the buffer for the next loop
      byte_buffer[i] = 0x00;
    rtcm1008_injected = false;                                          // set the 1008 message flag to false
    }
    Serial.println(F("reconnect"));
    delay(1000);                                                        // prevents reconnect spam
    if (!ntrip_s.subStation(host, httpPort, mountPoint, password, sourceString)) {      // try to reconnect
      delay(1000);                                                      // prevents reconnect spam
    }
    else {
      Serial.println(F("MountPoint Subscribed"));
      delay(10);
    }
  }
  delay(1);                                                            // minimum cycle time (ms).
}
