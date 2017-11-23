/*
  AMASP.h - Library that implements the AMASP - ASCII Master Slave Protocol (version 0.4).
  Created by Andre Luiz Delai, November 17, 2017.
*/
#ifndef AMASP_h
#define AMASP_h

#include "Arduino.h"

#define MSGMAXSIZE 128;

enum PacketType{MRP, SRP, SIP, CEP, None};

class AMASPSerialMaster
{
  public:
    AMASPSerialMaster();
    void begin(HardwareSerial *serial);
    void end();
    int sendRequisition(int deviceID, byte message[], int msgLength);
    void sendError(int code);
    PacketType readPacket(int *deviceID, byte message[], int *msgLength);
    
  private:
};

class AMASPSerialSlave
{
  public:
    AMASPSerialSlave();
    void begin(HardwareSerial *serial);
    void end();
    void sendResponse(int deviceID, byte* message, int msgLength);
    void sendInterruption(int code);
    void sendError(int code);
    void readPacket(PacketType *type, int *deviceID, byte message[], int *msgLength);

  private:   
};

//Auxiliary functions
void intToASCIIHex(int value, unsigned char hex[]);

int asciiHexToInt(unsigned char hex[]);

int LRC(byte* data, int dataLength);

#endif
