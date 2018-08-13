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


// Compute the MODBUS RTU CRC
short CRC16SerialModbus(byte* data, int dataLength)
{
  unsigned short crc = (short) 0xFFFF;
  for (int pos = 0; pos < dataLength; pos++) {
    crc ^= (unsigned short) data[pos];          // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--) // Loop over each bit
    {
      crc >>= 1;
      crc &= 0x7FFF;
      if ((crc & 0x0001) != 0) // If the LSB is set
      {
        crc ^= 0xA001; // Polynomial
      }
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;
}
