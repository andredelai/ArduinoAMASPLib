#include "Arduino.h"
#include "AMASP.h"


void AMASPSerialSlave::begin(HardwareSerial &serial)
{
  slaveCom = &serial;
}

void AMASPSerialSlave::end()
{
  slaveCom = NULL;
}

void AMASPSerialSlave::sendResponse(int deviceID, byte message[], int msgLength)
{
  char hex[sizeof(int) * 2];

  //Mounting the packet
  byte pkt[MSGMAXSIZE + 15];

  //Packet Type
  pkt[0] = '!';
  pkt[1] = '#';
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
  slaveCom->write(pkt, 15 + msgLength);
}

void AMASPSerialSlave::sendInterruption(int deviceID, int code)
{
  char hex[sizeof(int) * 2];
  byte pkt[14];

  //Packet Type
  pkt[0] = '!';
  pkt[1] = '!';
  //ECA
  intToASCIIHex(errorCheckAlg, hex);
  pkt[2] = hex[0];
  //Device ID
  intToASCIIHex(deviceID, hex);
  pkt[3] = hex[2];
  pkt[4] = hex[1];
  pkt[5] = hex[0];
  //Interruption Code
  //intToASCIIHex(errorCode, hex);
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

  slaveCom->write(pkt, 14);
}

void AMASPSerialSlave::sendError(int deviceID, int errorCode)
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

  slaveCom->write(pkt, 14);
}

PacketType AMASPSerialSlave::readPacket(int &deviceID, byte message[], int &codeLength)
{
    byte buf[MSGMAXSIZE + 15];
  PacketType type;
  ErrorCheck eca;
  int aux;

  //Searching for a packet in serial buffer (starts with !).
  while (slaveCom->readBytes(buf, 1) != 0)
  {
    if (buf[0] == '!')
    {
      //slaveCom->print("Preambulo detectado\r\n");
      //Reading packet type, ECA and device ID bytes
      if (slaveCom->readBytes(&buf[1], 5) != 5)
      {
        return Timeout;
      }
      if (buf[2] < '0' || buf[3] > '5')
      {
        return Timeout;
      }
      
      //slaveCom->print("Extraindo ECA\r\n");
      //Extracting ECA
      eca = asciiHexToInt(&buf[2], 1);

      //Extracting device ID
      deviceID = (int)asciiHexToInt(&buf[3], 3);
      if (deviceID == -1)
      {
        return Timeout;
      }

      //Verifing packet type
      switch (buf[1])
      {
        //SRP Packet detected ----------
        case '?':
          //slaveCom->print("Tipo de pacote detectado = ? \r\n");
          //Reading device ID and msg length
          if (slaveCom->readBytes(&buf[6], 3) != 3)
          {
            return Timeout;
          }
          //Extracting message length
          codeLength = asciiHexToInt(&buf[6], 3);
          if (codeLength == -1)
          {
            //slaveCom->print("Erro extração tam pacote\r\n");
            return Timeout;
          }
          //Checking the packet size value
          if (codeLength == 0 || codeLength > MSGMAXSIZE)
          {
            //slaveCom->print("Erro de valor tam de pacote\r\n");
            return Timeout;
          }
          //Reading message, error checking and end packet bytes
          if (slaveCom->readBytes(&buf[9], (codeLength) + 6) != (codeLength) + 6)
          {
            //slaveCom->print("Error de leitura do payload e resto do pacote\r\n");
            return Timeout;
          }

          //Extracting error checking bytes
          aux = asciiHexToInt(&buf[(codeLength) + 9], 4);
          if (aux == -1)
          {
            //slaveCom->print("Erro de extração do valor de checagem de erro\r\n");
            return Timeout;
          }

          // Error checking
          if (aux != errorCheck(buf, codeLength + 8, eca))
          {
            //slaveCom->print("Falha na checagem de erro\r\n");
            return Timeout;
          }

          //Checking the packet end
          if (buf[codeLength + 13] != '\r' ||  buf[codeLength + 14] != '\n')
          {
            slaveCom->print("Caracteres de fim de pacote não reconhecidos\r\n");
            return Timeout;
          }

          //Extracting message
          memcpy(message, &buf[9], codeLength);

          //SRP recognized
          return MRP;

          break;

        //SIP Packet detected ----------
        case '!':
          if (slaveCom->readBytes(&buf[6], 8) != 8)
          {
            return Timeout;
          }

          //Extracting error checking bytes
          aux = asciiHexToInt(&buf[8], 4);
          if (aux == -1)
          {
            return Timeout;
          }

          //error checking
          if (aux != errorCheck(buf, 8, eca))
          {
            return Timeout;
          }
          
          //Reading interrupt code
          codeLength = asciiHexToInt(&buf[6], 2);
          if (codeLength == -1)
          {
            return Timeout;
          }

          //Checking the packet end
          if (buf[12] != '\r' ||  buf[13] != '\n')
          {
            return Timeout;
          }
          
          return SIP; //SIP recognized
          break;

        //CEP Packet detected ----------
        case '~':
          if (slaveCom->readBytes(&buf[6], 8) != 8)
          {
            return Timeout;
          }

          //Extracting error checking bytes
          aux = asciiHexToInt(&buf[8], 4);
          if (aux == -1)
          {
            slaveCom->println("Error check bytes extraction error");
            return Timeout;
          }

          //error checking
          if (aux != errorCheck(buf, 8, eca))
          {
            slaveCom->println(aux);
            slaveCom->println(eca);
            slaveCom->println("Error check fail");
            return Timeout;
          }
          
          //Reading interrupt code
          codeLength = asciiHexToInt(&buf[6], 2);
          if (codeLength == -1)
          {
            slaveCom->println("Interrupt code extraction error");
            return Timeout;
          }

          //Checking the packet end
          if (buf[12] != '\r' ||  buf[13] != '\n')
          {
            slaveCom->println("End packet chars error");
            return Timeout;
          }
          
          return CEP; //SIP recognized
          break;

          default:
          return Timeout;
          break;
      }
    }
  }
  return Timeout;
  
}

void AMASPSerialSlave::SetErrorCheck(ErrorCheck errorChk)
{
  errorCheckAlg = errorChk;
}
