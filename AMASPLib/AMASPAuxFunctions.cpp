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

short XORCheck(byte* data, int dataLength)
{
  byte xorCheck = 0;
  for (int i = 0; i < dataLength; i++)
  {
    xorCheck ^= data[i];
  }
  return (short)xorCheck;
}


//Classical checksum
short checksum16Check(byte message[], int dataLength)
{
  unsigned short sum = 0;
  while (dataLength-- > 0)
  {
    sum += *(message++);
  }
  return (sum);
}   /* Sum() */

// LRC 16 bit checksum
short LRC16Check(byte* data, int dataLength)
{
  unsigned short lrc = 0;
  for (int i = 0; i < dataLength; i++)
  {
    lrc = (lrc + data[i]) & 0xFFFF;
  }
  lrc = (((lrc ^ 0xFFFF) + 1) & 0xFFFF);
  return lrc;
}

// CRC16 check
short CRC16SerialModbus(byte* data, int dataLength)
{
  unsigned short crc = (short) 0xFFFF;
  for (int pos = 0; pos < dataLength; pos++) {
    crc ^= (unsigned short) data[pos];          // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--) // Loop over each bit
    {
      if ((crc & 0x0001) != 0) // If the LSB is set
      {
        crc >>= 1;
        crc ^= 0xA001; // Polynomial
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;
}

// Fletcher 16 bit checksum
short fletcher16Checksum(byte *data, int dataLength)
{
  uint32_t c0, c1;
  unsigned int i;

  for (c0 = c1 = 0; dataLength >= 5802; dataLength -= 5802) {
    for (i = 0; i < 5802; ++i) {
      c0 = c0 + *data++;
      c1 = c1 + c0;
    }
    c0 = c0 % 255;
    c1 = c1 % 255;
  }
  for (i = 0; i < dataLength; ++i) {
    c0 = c0 + *data++;
    c1 = c1 + c0;
  }
  c0 = c0 % 255;
  c1 = c1 % 255;
  return (c1 << 8 | c0);
}

short errorCheck(byte *data, int dataLength, int errorCheckAlg)
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
      return fletcher16Checksum(data, dataLength);
      break;

    case 5:
      return CRC16SerialModbus(data, dataLength);
      break;

    default:
      return 0x00;
      break;

  }
}
