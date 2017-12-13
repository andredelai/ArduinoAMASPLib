//This code is a kind of loopback for all AMASP received packets.
//Arduino in slave mode

#include "AMASP.h"
#include "Arduino.h"

//AMASP Packet types returned by readPacket function.
//MRP - Master Requisition Packet
//SRP - Slave Response Packet
//SIP - Slave Interrupt Packet
//CEP - Communication Error Packet
//Timeout - No packet found


AMASPSerialSlave slave;
int device = 0x000;
int codeLength = 0x000;
PacketType type;
byte data[MSGMAXSIZE];

void setup()
{
  //Serial and AMASP setup
  Serial.begin(9600);
  Serial.flush();
  slave.begin(Serial); 
}

void loop() {
  // Listening serial interface...
  if (Serial.available() > 0)
  {
    type = slave.readPacket(device, data, codeLength);
    switch (type)
    {
      case MRP:        
        slave.sendResponse(device, data, codeLength);
        break;
      
      case CEP:
        //Echo
        slave.sendError(device, codeLength);
        break;

      case SIP:
        slave.sendInterruption(device, codeLength);
        break;

      case Timeout:
        Serial.println("Timeout");
        break;
    }
  }
}
