/*
  AMASP.h - Library that implements the AMASP - ASCII Master Slave Protocol (version 0.4).
  Created by Andre Luiz Delai, November 17, 2017.
*/
#ifndef AMASP_h
#define AMASP_h

#include "Arduino.h"

#define MSGMAXSIZE 256
#define PKTMAXSIZE MSGMAXSIZE + 14

enum PacketType {MRP, SRP, SIP, CEP, Timeout, Error};

class AMASPSerialMaster
{
  public:
    AMASPSerialMaster();
    void begin(HardwareSerial *serial);
    void end();
    int sendRequisition(int deviceID, byte message[], int msgLength);
    void sendError(int device, int errorCode);
    PacketType readPacket(int *deviceID, byte message[], int *codeLength);

  private:

};

class AMASPSerialSlave
{
  public:
    AMASPSerialSlave();
    void begin(HardwareSerial *serial);
    void end();
    void sendResponse(int deviceID, byte message[], int msgLength);
    void sendInterruption(int deviceID, int code);
    void sendError(int Device, int code);
    PacketType readPacket(int *deviceID, byte message[], int *codeLength);

  private:
   
};

//Auxiliary functions
void intToASCIIHex(int value, unsigned char hex[]);

long asciiHexToInt(unsigned char hex[], int length);

long LRC(byte* data, int dataLength);

#endif
