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
  masterCom = NULL;
}


int AMASPSerialMaster::sendRequest(int deviceID, byte message[], int msgLength)
{
  char hex[sizeof(int) * 2];

  //Mounting the packet
  byte pkt[MSGMAXSIZE + 15];

  //Packet Type
  pkt[0] = '!';
  pkt[1] = '?';
  //ECA
  intToASCIIHex(errorCheckAlg, hex);
  pkt[2] = hex[0];
  //Device ID
  intToASCIIHex(deviceID, hex);
  pkt[3] = hex[2];
  pkt[4] = hex[1];
  pkt[5] = hex[0];
  //Message Length
  intToASCIIHex(msgLength, hex);
  pkt[6] = hex[2];
  pkt[7] = hex[1];
  pkt[8] = hex[0];
  //Message (payload)
  for (int i = 0; i < msgLength ; i++)
  {
    pkt[9 + i] = message[i];
  }
  //Error checking algorithm
  //masterCom->write(pkt, msgLength + 9);
  intToASCIIHex(errorCheck(pkt, msgLength + 9, errorCheckAlg), hex);
  //intToASCIIHex(CRC16SerialModbus(pkt, msgLength + 8), hex);
  pkt[9 + msgLength] = hex[3];
  pkt[9 + msgLength + 1] = hex[2];
  pkt[9 + msgLength + 2] = hex[1];
  pkt[9 + msgLength + 3] = hex[0];
  //Packet End
  pkt[9 + msgLength + 4] = '\r';
  pkt[9 + msgLength + 5] = '\n';

  //sending Request
  masterCom->write(pkt, 15 + msgLength);

}

void AMASPSerialMaster::sendError(int deviceID, int errorCode)
{
  char hex[sizeof(int) * 2];
  byte pkt[14];

  //Packet Type
  pkt[0] = '!';
  pkt[1] = '~';
  //ECA
  intToASCIIHex(errorCheckAlg, hex);
  pkt[2] = hex[0];
  //Device ID
  intToASCIIHex(deviceID, hex);
  pkt[3] = hex[2];
  pkt[4] = hex[1];
  pkt[5] = hex[0];
  //Error Code
  intToASCIIHex(errorCode, hex);
  pkt[6] = hex[1];
  pkt[7] = hex[0];
  //Error checking algorithm
  intToASCIIHex(errorCheck(pkt, 8, errorCheckAlg), hex);
  pkt[8] = hex[3];
  pkt[9] = hex[2];
  pkt[10] = hex[1];
  pkt[11] = hex[0];
  //Packet End
  pkt[12] = '\r';
  pkt[13] = '\n';

  masterCom->write(pkt, 13);
}

PacketType AMASPSerialMaster::readPacket(int &deviceID, byte message[], int &codeLength)
{
  byte buf[MSGMAXSIZE + 15];
  PacketType type;
  byte *endPktPtr;
  int aux;

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
          if (masterCom->readBytes(&buf[3], 6) == 6)
          {
            //Extracting device ID
            deviceID = (int)asciiHexToInt(&buf[3], 3);
            if (deviceID != -1)
            {
              //Extracting message length
              codeLength = asciiHexToInt(&buf[6], 3);
              if (codeLength != -1)
              {
                //Checking the packet size limit
                if (codeLength <= MSGMAXSIZE || codeLength != 0)
                {
                  //Reading message, CRC16 and end packet chars
                  if (masterCom->readBytes(&buf[8], (codeLength) + 6) == (codeLength) + 6)
                  {
                    //error checking
                    aux = asciiHexToInt(&buf[(codeLength) + 8], 4);
                    if (aux != -1)
                    {
                      if (aux == errorCheck(buf, codeLength + 8, errorCheckAlg))
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
            //CRC16 check

            if (aux == errorCheck(buf, 7, errorCheckAlg))
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
            //CRC16 check
            
            if (aux == errorCheck(buf, 8, errorCheckAlg))
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

void AMASPSerialMaster::SetErrorCheck(ErrorCheck errorChk)
{
  errorCheckAlg = errorChk;
}
