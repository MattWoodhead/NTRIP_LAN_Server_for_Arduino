/*
 * Modified from https://github.com/GLAY-AK2/NTRIP-server-for-Arduino
 * Copyright GLAY-AK2, MattWoodhead
 */


#include"NTRIPServer.h"

bool NTRIPServer::subStation(char* host,int port,char* mountpoint,char* password, char* info)  //&port
{ 
  if(!connect(host, port))return false;
  char buff[256 + NTRIP_MAXSTR], *p = buff;
    Serial.println("Request NTRIP");
    
  p += sprintf(p, "SOURCE %s %s\r\n", password, mountpoint);
  p += sprintf(p, "Source-Agent: NTRIP LAN Server for Arduino V1.0\r\n");
  p += sprintf(p, "STR: %s\r\n", info);
  p += sprintf(p, "\r\n");

    print(buff);
    #ifdef Debug
    Serial.println(p);
    #endif
    unsigned long timeout = millis();
    while (available() == 0) {
        if (millis() - timeout > 20000) {
            Serial.println("Client Timeout !");
            return false;
        }
        delay(10);
    }
    char buffer[50];
    read((uint8_t*)buffer,sizeof(buffer));
    if(strncmp((char*)buffer,"ICY 200 OK",10))
    {
      Serial.print((char*)buffer);
      return false;
    }
    return true;
}

 // String NTRIPServer::makeOptionalSTR(char* dataFormat, char* details, char* carrier, char* navSys, char* network, country,char* lat,char*lon,char* nmea,char sol,char gen,comEnc,char* auth,fee,bitrate);
