/*
  AMASP.h - AMASP (ASCII MAster Slave Protocol) library for Arduino (version 0.9).
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
//MRP - Master Request Packet
//SRP - Slave Response Packet
//SIP - Slave Interrupt Packet
//CEP - Communication Error Packet
//Timeout - No packet found
enum PacketType {MRP = 0, SRP, SIP, CEP, Timeout};

//Serial Master class
class AMASPSerialMaster
{
  public:
    AMASPSerialMaster();
    //Starts the master
    void begin(HardwareSerial &serial);
    //Finalize the master (close the communication)
    void end();
    //Send a MRP packet
    int sendRequest(int deviceID, byte message[], int msgLength);
    //Send a CEP packet
    void sendError(int device, int errorCode);
    //Search for received packets
    PacketType readPacket(int &deviceID, byte message[], int &codeLength);

  private:
    HardwareSerial *masterCom = NULL;
};

//Serial Slave class
class AMASPSerialSlave
{
  public:
    AMASPSerialSlave();
    //Starts the slave
    void begin(HardwareSerial &serial);
    //Finalize the slave (close the communication)
    void end();
    //Send a SRP packet
    void sendResponse(int deviceID, byte message[], int msgLength);
    //Send a SIP packet
    void sendInterruption(int deviceID, int code);
    //Send a CEP packet
    void sendError(int Device, int code);
    //Search for received packets
    PacketType readPacket(int &deviceID, byte message[], int &codeLength);

  private:
    HardwareSerial *slaveCom = NULL;
};

//Auxiliary functions***********

//Dec int to ASCII hex conversion
void intToASCIIHex(int value, char hex[]);

//ASCII Hex to a long decimal conversion
long asciiHexToInt(char hex[], int length);

// Longitudinal Redundancy Check function (16 bits)
long LRC(byte* data, int dataLength);

#endif
