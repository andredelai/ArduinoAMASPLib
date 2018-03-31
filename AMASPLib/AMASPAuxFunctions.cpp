#include "Arduino.h"
#include "AMASP.h"

#define POLYNOM  0x3D65    // DNP, IEC 870, M-BUS, wM-BUS, ...

void intToASCIIHex(int value, char hex[])
{
  int i;
  for (i = 0; i < sizeof(int) * 2; i++)
  {
    hex[i] = (value >> (i * 4)) & 0x0F;
    hex[i] += hex[i] >= 0x0A ? ('A' - 10) : '0';
  }
}

long asciiHexToInt(char hex[], int length)
{
  long num = 0, i;

  for (i = 0; i < length  ; i++)
  {
    //Avoiding lowcase
    hex[i] = toupper(hex[i]);
    if (((hex[i] >= '0') && (hex[i] <= '9')) || ((hex[i] >= 'A') && (hex[i] <= 'F')))
    {
      num = hex[i] >= 'A' ? (num << 4) | (hex[i] - ('A' - 0x0A)) : (num << 4) | (hex[i] - '0');
    }
    else
    {
      //The string is not hexadecimal.
      return -1;
    }
  }
  return num;
}

long LRC(byte* data, int dataLength)
{
  long lrc = 0;
  for (int i = 0; i < dataLength; i++)
  {
    lrc = (lrc + data[i]) & 0xFFFF;
  }
  lrc = (((lrc ^ 0xFFFF) + 1) & 0xFFFF);
  return lrc;
}

int CRC16_DNP(unsigned int crcValue, unsigned char newByte) 
{
  unsigned char i;

  for (i = 0; i < 8; i++) {

    if (((crcValue & 0x8000) >> 8) ^ (newByte & 0x80))
    {
      crcValue = (crcValue << 1)  ^ POLYNOM;
    }
    else
    {
      crcValue = (crcValue << 1);
    }

    newByte <<= 1;
  }
  
  return crcValue;
}

int CRC16(byte* data, int dataLength)
{
  unsigned int crc;
  unsigned char aux = 0;

  crc = 0x0000; //Initialization of crc to 0x0000 for DNP

  while (aux < dataLength)
  {
    crc = CRC16_DNP(crc,data[aux]);
    aux++;
  }
  
  return (~crc); //The crc value for DNP it is obtained by NOT operation
}
