//

#include <iostream>
 
using namespace std;

#include <ESP8266WiFi.h>
#include <stdlib.h>
#include <stdio.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <string.h>
#include <time.h>

#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EEPROM.h>

#define pin D0
WiFiUDP Udp;
NTPClient nows(Udp, "2.asia.pool.ntp.org", 7*3600);

// WiFiUDP Udp;
class DVlight{
  protected:
      
      char* incomingPacket;
      String id;
      String watt;
      String mode;
      String timer;
      String dcode;
      bool isOn;
  public:

      DVlight();
      ~DVlight();
      void ReceiveCode(DVlight &DV, WiFiUDP Udp);
      void GetCode(DVlight &DV);
      void DeCode(DVlight &DV);
      bool TurnOn(DVlight &DV);
      bool TurnOff(DVlight &DV);
      void LightMode(DVlight &DV);
      bool SetTimer(DVlight &DV, NTPClient &nows);

      char* getIncomingPacket(DVlight &DV);
      bool getisOn(DVlight &DV);
      
      String charToString(char S[]);
};