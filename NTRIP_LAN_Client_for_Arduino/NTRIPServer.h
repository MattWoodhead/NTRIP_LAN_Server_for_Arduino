/*
 * Modified from https://github.com/GLAY-AK2/NTRIP-server-for-Arduino
 * Copyright GLAY-AK2, MattWoodhead
 */

#ifndef NTRIPSERVER_H_
#define NTRIPSERVER_H_

#include <Arduino.h>
#include <Ethernet.h>

#define NTRIP_MAXSTR 256

class NTRIPServer : public EthernetClient{
  public :
  bool subStation(char* host,int port,char* mntpnt,char* psw, char* info);      //request RAW data from Caster 
  //String makeOptionalSTR(char* dataFormat, char* details, char* carrier, char* navSys, char* network, country,char* lat,char*lon,char* nmea,char sol,char gen,comEnc,char* auth,fee,bitrate);
};

#endif //NTRIPSERVER_H_
