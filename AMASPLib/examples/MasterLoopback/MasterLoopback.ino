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


AMASPSerialMaster master; //AMASP master instance
int device = 0x000;
int codeLength = 0x000;
PacketType type;
byte data[MSGMAXSIZE];

void setup()
{
  //Serial and AMASP setup
  Serial.begin(9600);
  //Clear the serial
  Serial.flush();
  //Connect the slave to the serial
  master.begin(Serial);

  //No error checking
  master.setErrorCheck(none);
}

void loop() {
  // Listening serial interface...
  if (Serial.available() > 0)
  {
    type = master.readPacket(device, data, codeLength);
    switch (type)
    {     
      case SRP:
        //Response to a SRP will be a MRP with same content (just for tests)
        master.sendRequest(device, data, codeLength);
        break;

      case SIP:
        //Response to a SIP will be a CEP with same content (just for tests)
        master.sendError(device, codeLength);
        break;

      case CEP:
        //Response to a SIP will be a CEP with same content (just for tests)
        master.sendError(device, codeLength);
        break;
        
      case Timeout:
        Serial.println("Timeout");
        break;
    }
  }
}
