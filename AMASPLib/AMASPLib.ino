//This code is a kind of loopback for all AMASP received packets.
//Arduino in SLAVE mode

#include "AMASP.h"
#include "Arduino.h"

//AMASP Packet types returned by readPacket function.
//MRP - Master Request Packet
//SRP - Slave Response Packet
//SIP - Slave Interrupt Packet
//CEP - Communication Error Packet
//Timeout - No packet found

//AMASPSerialMaster master;
AMASPSerialSlave slave;
int device = 0x00A;
int codeLength = 0x000;
PacketType type;
ErrorCheck eca;
byte data[MSGMAXSIZE];
byte payload[20] {'H','E','L','L','O'};

void setup()
{
  short ret;
  //Serial and AMASP setup
  Serial.begin(9600);
  Serial.flush();
  slave.begin(Serial);
  //master.begin(Serial);

  //ret = XORCheck2(payload, 14);
  //Serial.print(ret);
  
  //master.SetErrorCheck(none);
//  master.sendRequest(device, payload, 5);
//  
  //slave.SetErrorCheck(XOR8);
  slave.sendError(device, 0x05);
//  master.sendRequest(device, payload, 5);
//
//  master.SetErrorCheck(checksum16);
//  master.sendRequest(device, payload, 5);
//
//  master.SetErrorCheck(LRC16);
//  master.sendRequest(device, payload, 5);

//  master.SetErrorCheck(fletcher16);
//  master.sendRequest(device, payload, 5);

   // master.SetErrorCheck(none);
   // master.sendError(device, 0xA2);
   
}

void loop() {
 
  //Listening serial interface...
  if (Serial.available() > 0)
  {
    type = slave.readPacket(device, data, codeLength, eca);
    slave.SetErrorCheck(eca);
    switch (type)
    {
      case SRP:        
        slave.sendResponse(device, data, codeLength);
        break;
      
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
        //Serial.println("Timeout");
        break;
    }
  }
}
