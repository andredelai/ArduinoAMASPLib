#include "Arduino.h"
#include "AMASP.h"


//Constructor
AMASPSerialMaster::AMASPSerialMaster()
{

}



void AMASPSerialMaster:: begin(HardwareSerial &serial)
{
  masterCom = &serial;
}

void AMASPSerialMaster::end()
{
  if (masterCom != NULL)
  {
    masterCom->end();
  }
}

int AMASPSerialMaster::sendRequest(int deviceID, byte message[], int msgLength)
{
  char hex[sizeof(int) * 2];

  //Mounting the packet
  byte pkt[PKTMAXSIZE];
  //byte* pkt = new byte[14 + msgLength];

  //Packet Type
  pkt[0] = '!';
  pkt[1] = '?';
  //Device ID
  intToASCIIHex(deviceID, hex);
  pkt[2] = hex[2];
  pkt[3] = hex[1];
  pkt[4] = hex[0];
  //Message Length
  intToASCIIHex(msgLength, hex);
  pkt[5] = hex[2];
  pkt[6] = hex[1];
  pkt[7] = hex[0];
  //Message (payload)
  for (int i = 0; i < msgLength ; i++)
  {
    pkt[8 + i] = message[i];
  }
  //LRC
  intToASCIIHex(LRC(pkt, msgLength + 8), hex);
  pkt[8 + msgLength] = hex[3];
  pkt[8 + msgLength + 1] = hex[2];
  pkt[8 + msgLength + 2] = hex[1];
  pkt[8 + msgLength + 3] = hex[0];
  //Packet End
  pkt[8 + msgLength + 4] = '\r';
  pkt[8 + msgLength + 5] = '\n';

  //sending Request
  masterCom->write(pkt, 14 + msgLength);

}

void AMASPSerialMaster::sendError(int deviceID, int errorCode)
{
  char hex[sizeof(int) * 2];
  byte pkt[13];

  //Packet Type
  pkt[0] = '!';
  pkt[1] = '~';
  //Device ID
  intToASCIIHex(deviceID, hex);
  pkt[2] = hex[2];
  pkt[3] = hex[1];
  pkt[4] = hex[0];
  //Error Code
  intToASCIIHex(errorCode, hex);
  pkt[5] = hex[1];
  pkt[6] = hex[0];
  //LRC
  intToASCIIHex(LRC(pkt, 7), hex);
  pkt[7] = hex[3];
  pkt[8] = hex[2];
  pkt[9] = hex[1];
  pkt[10] = hex[0];
  //Packet End
  pkt[11] = '\r';
  pkt[12] = '\n';

  masterCom->write(pkt, 13);
}

PacketType AMASPSerialMaster::readPacket(int &deviceID, byte message[], int &codeLength)
{
  byte buf[PKTMAXSIZE];
  PacketType type;
  byte *endPktPtr;
  long aux;

  //Searching for a packet in serial buffer (starts with !).
  while (masterCom->readBytes(buf, 1) != 0)
  {
    if (buf[0] == '!')
    {
      //Reading packet type
      if (masterCom->readBytes(&buf[1], 1) != 1)
      {
        return Timeout;
      }
      //Verifing type
      switch (buf[1])
      {
        //SRP Packet******
        case '#':
          //Reading device ID and msg length
          if (masterCom->readBytes(&buf[2], 6) == 6)
          {
            //Extracting device ID
            deviceID = asciiHexToInt(&buf[2], 3);
            if (deviceID != -1)
            {
              //Extracting message length
              codeLength = asciiHexToInt(&buf[5], 3);
              if (codeLength != -1)
              {
                //Checking the packet size limit
                if (codeLength <= MSGMAXSIZE || codeLength != 0)
                {
                  //Reading message, LRC and end packet chars
                  if (masterCom->readBytes(&buf[8], (codeLength) + 6) == (codeLength) + 6)
                  {
                    //LRC checking
                    aux = asciiHexToInt(&buf[(codeLength) + 8], 4);
                    if (aux != -1)
                    {
                      if (aux == LRC(buf, (codeLength) + 8))
                      {
                        //Checking the packet end
                        if (buf[codeLength + 12] == '\r' ||  buf[codeLength + 13] == '\n')
                        {
                          //Extracting message
                          memcpy(message, &buf[8], codeLength);
                          return SRP;//SRP recognized
                        }
                      }
                    }
                  }
                  else
                  {
                    return Timeout;
                  }

                }

              }
            }
          }
          else
          {
            return Timeout;
          }
          break;

        //SIP Packet******
        case '!':
          if (masterCom->readBytes(&buf[2], 11) != 11)
          {
            return Timeout;
          }
          aux = asciiHexToInt(&buf[7], 4);
          if (aux != -1)
          {
            //LRC check
            if (aux == LRC(buf, 7))
            {
              //Extracting device ID
              deviceID = asciiHexToInt(&buf[2], 3);
              if (deviceID != -1)
              {
                //Reading interrupt code
                codeLength = asciiHexToInt(&buf[5], 2);
                if (codeLength != -1)
                {
                  //Checking the packet end
                  if (buf[11] == '\r' ||  buf[12] == '\n')
                  {
                    return SIP; //SIP recognized
                  }
                }
              }
            }
          }
          break;

        //CEP Packet*****
        case '~':
          if (masterCom->readBytes(&buf[2], 11) != 11)
          {
            return Timeout;
          }
          aux = asciiHexToInt(&buf[7], 4);
          if (aux != -1)
          {
            //LRC check
            if (aux == LRC(buf, 7))
            {
              //Extracting device ID
              deviceID = asciiHexToInt(&buf[2], 3);
              if (deviceID != -1)
              {
                //Reading interrupt code
                codeLength = asciiHexToInt(&buf[5], 2);
                if (codeLength != -1)
                {
                  //Checking the packet end
                  if (buf[11] == '\r' ||  buf[12] == '\n')
                  {
                    return CEP; //CEP recognized
                  }
                }
              }
            }
          }
          break;

        default:
          //It´s not a valid packet, the search continues...
          break;
      }
    }
  }
  masterCom->print("timeout");
  return Timeout;
}

