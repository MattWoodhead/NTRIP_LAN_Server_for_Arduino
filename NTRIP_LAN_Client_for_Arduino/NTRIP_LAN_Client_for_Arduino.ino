/* This script ans the associated files allow and Arduino Uno and an Ethernet shield to act as an NTRIP Server.
 * The intended hardware is an Arduino Uno, and Ethernet Shield V2, and a SimpleRTK2B stacked on top of each other.
 * 
 * Modified from https://github.com/GLAY-AK2/NTRIP-server-for-Arduino
 * Copyright GLAY-AK2, MattWoodhead
 */

#include "NTRIPServer.h"
//#include "SoftwareSerial.h"
#include <Ethernet.h>
#include <SPI.h>

// This is where the caster address, port, mopuntpoint, password etc are stored.
#include "NTRIPConfig.h"                                               

// SimpleRTK2B RX2 is connected to Mega RX1 (19)
HardwareSerial & RtcmSerial = Serial1;                              // this is assigning a name to the serial 1 port

// Randomly generated at https://www.hellion.org.uk/cgi-bin/randmac.pl 
const byte mac_address[] = {0xA8,0x61,0x0a,0xAE,0x7E,0x6C};
char char_buffer[512];                                                // buffer into which we will read the RtcmSerial port
int char_count = 0;                                                   // counter to keep track of buffer length

NTRIPServer ntrip_s;                                                  // Create an instance of the NTRIPServer class

void setup() {
  delay(500);
  Serial.begin(115200);
  Serial.println(F("Starting..."));
  RtcmSerial.begin(115200);                                            //57600 causes issues - crashes arduino uno during ethernet connection
  delay(250);
  
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);                                              // disable SD card
  
  delay(250);

  Ethernet.begin(mac_address);
  delay(100);
  while (Ethernet.linkStatus() == LinkOFF) {                        // Wait for ethernet to connect
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
}

void loop() {
  if (ntrip_s.connected()) {
    while (RtcmSerial.available()) {
      char c = RtcmSerial.read();                                       // read in a byte as a character
      char_buffer[char_count++] = c;                                    // add character to buffer and increment counter
      if ((c == '\n') || (char_count == sizeof(char_buffer)-1)) {       // if the buffer gets full or we see the newline character
        ntrip_s.write((uint8_t*)char_buffer, char_count);               // send buffer to the ntrip server
        char_count = 0;                                                 // reset counter
        for (int i=0;i<sizeof(char_buffer);i++){                        // wipe the buffer for the next loop
          char_buffer[i] = " ";
        //Serial.println(F("packet sent"));                             // for debugging only
        }
      }
    }
  } else {
    ntrip_s.stop();
    Serial.println(F("reconnect"));
    delay(1000);                                                        // prevents reconnect spam
    if (!ntrip_s.subStation(host, httpPort, mountPoint, password, sourceString)) {
      delay(100);
    }
    else {
      Serial.println(F("MountPoint Subscribed"));
      delay(10);
    }
  }
  delay(5);                                                            //server cycle
}
