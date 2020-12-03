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

//XOR8
uint16_t XORCheck(const uint8_t *data, int dataLength)
{
  uint8_t xorCheck = 0;
  for (int i = 0; i < dataLength; i++)
  {
    xorCheck ^= data[i];
  }
  return (uint16_t)xorCheck;
}

//Checksum16
uint16_t checksum16Check(const uint8_t *data, int dataLength)
{
  unsigned short sum = 0;
  while (dataLength-- > 0)
  {
    sum += *(data++);
  }
  return (sum);
}

// LRC16
uint16_t LRC16Check(const uint8_t *data, int dataLength)
{
  unsigned short lrc = 0;
  for (int i = 0; i < dataLength; i++)
  {
    lrc = (lrc + data[i]) & 0xFFFF;
  }
  lrc = (((lrc ^ 0xFFFF) + 1) & 0xFFFF);
  return lrc;
}

// CRC16Modbus
uint16_t CRC16SerialModbus(const uint8_t *data, int dataLength)
{
  unsigned short crc = (short) 0xFFFF; //initialization
  for (int pos = 0; pos < dataLength; pos++) {
    crc ^= (unsigned short) data[pos];

    for (int i = 8; i != 0; i--)
    {
      if ((crc & 0x0001) != 0)
      {
        crc >>= 1;
        crc ^= 0xA001; // Polynomial Modbus
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  return crc;
}

//Fletcher16
uint16_t fletcher16Check(const uint8_t *data, size_t dataLength) 
{
  uint32_t c0, c1;
  for (c0 = c1 = 0; dataLength > 0; ) {
    size_t blocksize = dataLength;
    if (blocksize > 0x138A) {
      blocksize = 0x138A;
    }
    dataLength -= blocksize;
    do {
      c0 += *data++;
      c1 += c0;
    } while (--blocksize);
    c0 %= 0xFF;
    c1 %= 0xFF;
   }
   return (c1 << 8 | c0);
}

uint16_t errorCheck(byte *data, int dataLength, int errorCheckAlg)
{
  switch (errorCheckAlg)
  {
    case 1:
      return XORCheck(data, dataLength);
      break;

    case 2:
      return checksum16Check(data, dataLength);
      break;

    case 3:
      return LRC16Check(data, dataLength);
      break;

    case 4:
      return fletcher16Check(data, dataLength);
      break;

    case 5:
      return CRC16SerialModbus(data, dataLength);
      break;

    default:
      return 0x00;
      break;

  }
}
