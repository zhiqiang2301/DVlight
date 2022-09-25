#include <DVlightfunc_ver1.h>

DVlight::DVlight()
{
};

DVlight::~DVlight()
{

};

char* DVlight::getIncomingPacket(DVlight &DV)
{
  return this->incomingPacket;
}
bool DVlight::getisOn(DVlight &DV)
{
  return this->isOn;
}
String DVlight::charToString(char S[])
{
  String rc(S); 
  return rc;
}
void DVlight::ReceiveCode(DVlight &DV, WiFiUDP Udp)

{
  
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bits from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(DV.incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    
    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write("Hi there! Got the message :-)");
    Udp.endPacket();
  }
};

void DVlight::GetCode(DVlight &DV)
{
  char idd[32];
  // int len = Udp.read(this->incomingPacket, 255);

// Get ID
  for (int i = 0; i < 16; i++)
  {
 
    idd[i] = incomingPacket[i];
    idd[16] = '\0';  
  }
  this->id = this->charToString(idd);
//  Serial.printf("ID = ");
//  Serial.println(lights.ID);
// Get Watt 
  for (int i = 16; i < 20; i++)
  {
    idd[i-16] = incomingPacket[i];
    idd[4] = '\0';
  }
      
      this->watt = this->charToString(idd);
//  Serial.printf("Watt = ");
//  Serial.println(lights.watt);
// Get LightMode
  for (int i = 20; i < 22; i++)
  {
    idd[i-20] = incomingPacket[i];
    idd[2] = '\0';
  }
    this->mode = this->charToString(idd);
//  Serial.printf("Light = ");
//  Serial.println(lights.lightmode);
// Time
  for (int i = 22; i < 40; i++)
  {
    idd[i-22] = incomingPacket[i];
    idd[18] = '\0';
  }
    this->timer= this->charToString(idd);
//  Serial.printf("Time = ");
//  Serial.println(lights.starttime);
  //-------------------------------------
  // IsOn

  {
    idd[0] = incomingPacket[40];
    idd[1] = '\0';
  }
    this->isOn = bool(this->charToString(idd));
//  Serial.printf("IsOn = ");
//  Serial.println(lights.IsOn);
  // Decode
  for (int i = 41; i < 44; i++)
  {
    idd[i-41] = incomingPacket[i];
    idd[3] = '\0';
  }
    this->dcode = this->charToString(idd);
//  Serial.printf("Decode = ");
//  Serial.println(lights.dcode);
}

bool DVlight::TurnOn(DVlight& DV)
{
   digitalWrite(pin, HIGH);
   bool checkon = true;
   this->isOn = true;

   return checkon;
}

bool DVlight::TurnOff(DVlight& DV)
{
   digitalWrite(pin, LOW);
   bool checkoff = true;
   this->isOn = false;
   
   return checkoff;
}

void DVlight::LightMode(DVlight &DV)
{
  
    if ((this->mode).length() > 0) {
      if  (this->mode == "00") {
        analogWrite(pin, 0);
//        Serial.printf("Mode 0");
//        Serial.println();
      }
      else if (this->mode == "01") {
        analogWrite(pin, 255);
//        Serial.printf("Mode 1");
//        Serial.println();
        
      }
      else if (this->mode == "10") {
        analogWrite(pin, 512);
//        Serial.printf("Mode 2");
//        Serial.println();
        
      }
      else {
        analogWrite(pin, 1023);
//        Serial.printf("Mode 3");
//        Serial.println();
      }
      
     Serial.printf("Complete");
//      Serial.println();
    }
    
    else {
      Serial.printf("Er");
//      Serial.println();
      }
}

bool DVlight::SetTimer(DVlight &DV, NTPClient &nows)
{

  nows.update();
  delay(1000);
  String temp = nows.getFormattedTime();
  Serial.println(temp);
  temp.remove(2,1);
  temp.remove(4,1);
  Serial.println(temp); 
  temp = String("1" + temp);
  long tmp = temp.toInt();
  temp = String(tmp-1000000, BIN);
  Serial.println(temp); 
  if (temp.length() == 16) String("0" + temp);
  Serial.println(temp);
  if (temp == this->timer)
    return true;
  else return false;

}

void DVlight::DeCode(DVlight &DV){
    

    if  (this->dcode == "000") {
      DV.TurnOn(DV);
    }
    else if  (this->dcode == "001") {
      DV.TurnOff(DV);
    }
    else if  (this->dcode == "010") {
      DV.LightMode(DV); 
      Serial.printf("OK");
    }
    // else if (this_>dcode == "111"){
    //   LightModeSS1();
    // }
    else if (this->dcode == "011") 
      if (SetTimer(DV,nows))
      {
        if (this->isOn == false){
          digitalWrite(pin,HIGH);
          exit(0);
        }
        else{
          digitalWrite(pin,LOW);
          exit(0);
        }
      }

}

