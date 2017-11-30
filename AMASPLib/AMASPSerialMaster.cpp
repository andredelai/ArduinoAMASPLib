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

void AMASPSerialMaster::sendError(int errorCode)
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
  PacketType type;
  byte *endPktPtr;
  int aux;
  
  if (masterCom->peek() == '!')
  {
    
    //masterCom->readBytesUntil('\n', buf, PKTMAXSIZE);
    //endPktPtr = memchr(buf,'\n',PKTMAXSIZE);
    if(masterCom->available()<13)
    {
      return Unknown;
    }
    
    aux = asciiHexToInt(endPktPtr-5,4);
    if(aux != LRC (buf, (endPktPtr - buf) + 1))
    {
      return None;
    }  
    switch (buf[1])
    {
      //SRP
      case '#':
      masterCom->ReadBytes(    
      //Calculating message length
      aux = asciiHexToInt(&buf[5], 3);
      break;
      //SIP
      case '!':
      {
          type = SIP;
          *deviceID = asciiHexToInt(&buf[2],3);
          *codeLength = asciiHexToInt(&buf[5],3);
      }
        break;
      //CEP'
      case '~':
        aux = asciiHexToInt(&buf[4], 4);
        type = CEP;
        *codeLength = asciiHexToInt(&buf[2],2);
        type = None;
        break;
      default:
          type = None;
        break;
    }
    return type;
  }

  
}
