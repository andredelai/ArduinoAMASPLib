#include "Arduino.h"
#include "AMASP.h"

HardwareSerial *masterCom;

AMASPSerialMaster::AMASPSerialMaster()
{

}

void AMASPSerialMaster:: begin(HardwareSerial *serial)
{
  masterCom = serial;
}

void AMASPSerialMaster::end()
{
  masterCom->end();
}

int AMASPSerialMaster::sendRequisition(int deviceID, byte message[], int msgLength)
{
  char hexa[sizeof(int) * 2];

  //mounting the packet
  byte* pkt = new byte[14 + msgLength];

  pkt[0] = '!';
  pkt[1] = '?';
  intToASCIIHex(deviceID, hexa);
  pkt[2] = hexa[2];
  pkt[3] = hexa[1];
  pkt[4] = hexa[0];
  intToASCIIHex(msgLength, hexa);
  pkt[5] = hexa[2];
  pkt[6] = hexa[1];
  pkt[7] = hexa[0];
  for (int i = 0; i < msgLength ; i++)
  {
    pkt[8 + i] = message[i];
  }
  intToASCIIHex(LRC(pkt, 5 + msgLength + 3), hexa);
  pkt[8 + msgLength] = hexa[3];
  pkt[8 + msgLength + 1] = hexa[2];
  pkt[8 + msgLength + 2] = hexa[1];
  pkt[8 + msgLength + 3] = hexa[0];
  pkt[8 + msgLength + 4] = '\r';
  pkt[8 + msgLength + 5] = '\n';

  //sending requisition
  masterCom->write(pkt, 14 + msgLength);
  delete pkt;
}
