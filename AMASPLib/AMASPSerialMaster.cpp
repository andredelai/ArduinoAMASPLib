#include "Arduino.h"
#include "AMASP.h"

#define PKTMAXSIZE MSGMAXSIZE + 14

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
  byte pkt[PKTMAXSIZE];
  //byte* pkt = new byte[14 + msgLength];

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
  
}

void AMASPSerialMaster::sendError(int device, int errorCode)
{
  char hex[sizeof(int) * 2];
  byte pkt[10];
  pkt[0] = '!';
  pkt[1] = '~';
  intToASCIIHex(errorCode, hex);
  pkt[2] = hex[1];
  pkt[3] = hex[0];
  intToASCIIHex(LRC(pkt, 4), hex);
  pkt[4] = hex[3];
  pkt[5] = hex[2];
  pkt[6] = hex[1];
  pkt[7] = hex[0];
  pkt[8] = '\r';
  pkt[9] = '\n';

  masterCom->write(pkt, 10);
}

PacketType AMASPSerialMaster::readPacket(int *deviceID, byte message[], int *codeLength)
{
  byte buf[PKTMAXSIZE];
  bool keepReading = true;
  PacketType type;
  byte *endPktPtr;
  int aux;

  while (masterCom->readBytes(buf, 1) != 0 && keepReading == true)
  {
    if (buf[0] == '!')
    {
      //Reading pkt type and device ID
      masterCom->readBytes(&buf[1], 4);

      //Verifing type
      switch (buf[1])
      {
        //SRP
        case '#':
          //Reading msg length
          masterCom->readBytes(&buf[5], 3);
          //Reading message length
          *codeLength = asciiHexToInt(&buf[5], 3);
          //Reading message, LRC and end packet chars
          masterCom->readBytes(&buf[8], *codeLength + 6);
          //LRC checking
          aux = asciiHexToInt(&buf[aux + 9], 4);
          if (aux != LRC(&buf[aux + 9], 4))
          {
            return Error;
          }
          *deviceID = asciiHexToInt(&buf[2], 3);
          memcpy(message, &buf[8], *codeLength);
          if (buf[aux + 13] != '\r' ||  buf[aux + 14] != '\n')
          {
            return None;
          }
          return SRP;
          break;

        case '!':
          aux = asciiHexToInt(&buf[4], 4);
          if (aux != LRC(&buf[aux + 8], 4))
          {
            return Error;
          }
          //Reading device ID
          *deviceID = asciiHexToInt(&buf[2], 3);
          //Reading interrupt code
          *codeLength = asciiHexToInt(&buf[5], 2);
          if (buf[aux + 11] != '\r' ||  buf[12] != '\n')
          {
            return Error;
          }
          return SIP;
          break;

        case '~':
          //LRC checking
          aux = asciiHexToInt(&buf[4], 4);
          if (aux != LRC(&buf[aux + 8], 4))
          {
            return Error;
          }
          //Reading device ID
          *deviceID = asciiHexToInt(&buf[2], 3);
          //Reading error code
          *codeLength = asciiHexToInt(&buf[5], 2);
          if (buf[aux + 11] != '\r' ||  buf[12] != '\n')
          {
            return Error;
          }
          return CEP;
          break;

      }
    }
  }
  return None;
}

