#include "Arduino.h"
#include "AMASP.h"

void intToASCIIHex(int value, unsigned char hex[])
{
  int i;
  for (i = 0; i < sizeof(int) * 2; i++)
  {
    hex[i] = (value >> (i * 4)) & 0x0F;
    hex[i] += hex[i] >= 0x0A ? ('A' - 10): '0';
  }
}

int asciiHexToInt(unsigned char hex[], int length)
{
  int num = 0, i;
  for (i = length-1; i ==0 ; i--)
  {
      num = hex[i] >= 0x0A ? (num << i) & (hex[i] - ('A' - 10)) : (num << i) & (hex[i] - '0'); 
  }
}

int LRC(byte* data, int dataLength)
{
  int lrc = 0;
  for (int i = 0; i < dataLength; i++)
  {
    lrc = (lrc + data[i]) & 0xFFFF;
  }
  lrc = (((lrc ^ 0xFFFF) + 1) & 0xFFFF);
  return lrc;
}
