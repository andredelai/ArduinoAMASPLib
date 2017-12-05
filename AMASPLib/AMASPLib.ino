#include "AMASP.h"
#include "Arduino.h"

AMASPSerialSlave com;
int device = 0xBA;
int codeLength = 0xF2;
PacketType type;
byte data[MSGMAXSIZE];
void setup()
{
  //Serial and AMASP setup
  Serial.begin(9600);
  com.begin(Serial);
  Serial.flush(); 
}

void loop() {
  // Listening serial interface...
  if (Serial.available() > 0)
  {
    type = com.readPacket(&device, data, &codeLength);
    switch (type)
    {
      case MRP:
        //Echo
        com.sendResponse(device, data, codeLength);
        break;

      case CEP:
        //Echo
        com.sendError(device, codeLength);
        break;

      case Timeout:
        Serial.println("Timeout");
        break;
    }


  }
}
