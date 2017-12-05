/*
  AMASP.h - AMASP - ASCII Master Slave Protocol library for Arduino (version 0.5).
  Created by Andre Luiz Delai, November 17, 2017.
  Copyright (c) 2017 Andre L. Delai.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef AMASP_h
#define AMASP_h

#include "Arduino.h"

#define MSGMAXSIZE 128 //Maximum number of bytes per message on Arduino.
#define PKTMAXSIZE MSGMAXSIZE + 14 //Maximum packet size in bytes.

//AMASP Packet types returned by readPacket function.
enum PacketType {MRP = 0, SRP, SIP, CEP, Timeout};

//Serial Master class
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
    HardwareSerial *masterCom = NULL;
};

//Serial Slave class
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
    HardwareSerial *slaveCom = NULL;
};

//Auxiliary functions***********
void intToASCIIHex(int value, unsigned char hex[]);

long asciiHexToInt(unsigned char hex[], int length);

long LRC(byte* data, int dataLength);

#endif
