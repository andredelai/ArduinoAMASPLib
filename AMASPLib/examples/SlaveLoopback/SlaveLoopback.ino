//Created by Andre Luiz Delai, November 20, 2017.

//AMASP is a very simple protocol based on four packet types to communication between a Master and a Slave computer.

//This code is a kind of loopback for all AMASP received packets.

//Arduino here is in SLAVE mode

#include <AMASP.h>

//AMASP Packet types.
//MRP - Master Request Packet
//SRP - Slave Response Packet
//SIP - Slave Interrupt Packet
//CEP - Communication Error Packet
//Timeout - No packet found


AMASPSerialSlave slave; //AMASP slave instance
int device = 0x000;
int codeLength = 0x000;
PacketType type;
byte msg[MSGMAXSIZE];

void setup()
{
  //Serial and AMASP setup
  Serial.begin(9600);
  //Clear the serial 
  Serial.flush();
  //Connect the slave to the serial
  slave.begin(Serial);

  //No error checking
  slave.setErrorCheck(none);
}

void loop() {
  // Listening serial interface...
  if (Serial.available() > 0)
  {
    type = slave.readPacket(device, msg, codeLength);
    switch (type)
    {
      case MRP:        
        slave.sendResponse(device, msg, codeLength);
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
