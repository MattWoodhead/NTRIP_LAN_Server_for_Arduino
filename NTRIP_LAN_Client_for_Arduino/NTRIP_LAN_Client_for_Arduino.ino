/* This script ans the associated files allow and Arduino Uno and an Ethernet shield to act as an NTRIP Server.
 * The intended hardware is an Arduino Uno, and Ethernet Shield V2, and a SimpleRTK2B stacked on top of each other.
 * 
 * Modified from https://github.com/GLAY-AK2/NTRIP-server-for-Arduino
 * Copyright GLAY-AK2, MattWoodhead
 */

#include "NTRIPServer.h"
#include "SoftwareSerial.h"

#include "NTRIPConfig.h"  // This is where the caster address, port, mopuntpoint, password etc are stored.

// Setup software serial port to receive UART2 communication from the SimpleRTK2b
SoftwareSerial RtcmSerial(8, 2); // RX, TX

// Randomly generated at https://www.hellion.org.uk/cgi-bin/randmac.pl 
byte mac_address[] = {0x34,0x61,0x3a,0x62,0x31,0x3a};

NTRIPServer ntrip_s;  // Create an 

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  RtcmSerial.begin(38400);  //57600 causes issues - crashes arduino uno during ethernet connection
  delay(500);
  
  Ethernet.begin(mac_address);
  while (Ethernet.linkStatus() == LinkOFF) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("LAN connected");
  Serial.println("IP address: ");
  Serial.println(Ethernet.localIP());

  Serial.println("Subscribing MountPoint");
  delay(100);
  if (!ntrip_s.subStation(host, httpPort, mountPoint, password, sourceString)) {
    delay(15000);
  }
  Serial.println("Subscribing MountPoint is OK");
}

char ch[512];
int readcount;

void loop() {
  // put your main code here, to run repeatedly:
  if (ntrip_s.connected()) {
    while (RtcmSerial.available()) {
      readcount = 0;
      while (RtcmSerial.available()) {
        ch[readcount] = RtcmSerial.read();
        readcount++;
        if (readcount > 511)break;
      }//buffering
      ntrip_s.write((uint8_t*)ch, readcount);
    }
  }
  else {
    ntrip_s.stop();
    Serial.println("reconnect");
    Serial.println("Subscribing MountPoint");
    if (!ntrip_s.subStation(host, httpPort, mountPoint, password, sourceString)) {
      delay(100);
    }
    else {
      Serial.println("Subscribing MountPoint is OK");
      delay(10);
    }

  }
  delay(10);  //server cycle
}
