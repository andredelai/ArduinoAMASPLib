/*
  AMASP.h - Library that implements the AMASP - ASCII Master Slave Protocol (version 0.4).
  Created by Andre Luiz Delai, November 17, 2017.
  
*/
#ifndef AMASP_h
#define AMASP_h

#include "Arduino.h"

class AMASPSerialMaster
{
  public:
    AMASPSerialMaster();
    void begin(HardwareSerial *serial);
    void end();
    int sendRequisition(int deviceID, byte* message, int msgLength);
    
  private:
};

class AMASPSerialSlave
{
  public:
    AMASPSerialSlave();
    void begin(int portNumber);
    void end();
    bool sendResponse(int deviceID, byte* message, int msgLength);
    bool sendInterruption(int code);
    bool sendError(int code);

  private:   
};

void intToASCIIHex(int value, unsigned char hex[]);

int asciiHexToInt(unsigned char hex[]);

int LRC(byte* data, int dataLength);

#endif
