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

/// <summary>
/// Maximum number of bytes per message on Arduino.
/// </summary>
#define MSGMAXSIZE 128
/// <summary>
/// //Maximum packet size in bytes.
/// </summary>
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
    /// <summary>
    /// Initializes the master connecting it to the serial link.
    /// </summary>
    /// <param name="serial">Serial communication object. </param>
    void begin(HardwareSerial &serial);
    /// <summary>
    /// Finalizes the master disconnect it from the serial link.
    /// </summary>
    void end();
    //Send a MRP packet
    /// <summary>
    /// Send a MRP (Master Request Packet).
    /// </summary>
    /// <param name="deviceID">Device identification. </param>
    /// <param name="message">Message to be send to the associated device.</param>
    /// <param name="msgLength">Message length in bytes.</param>
    int sendRequest(int deviceID, byte message[], int msgLength);
    /// <summary>
    /// Send a CEP (Communication Error Packet).
    /// </summary>
    /// <param name="deviceID">Device identification. </param>
    /// <param name="errorCode">The communication error code.</param>
    void sendError(int device, int errorCode);
    /// <summary>
    /// Read the incoming AMASP packet.
    /// </summary>
    /// <param name="deviceID">Device identification. </param>
    /// <param name="message">Message read from the associated device.</param>
    /// <param name="msgLength">Message length in bytes.</param>
    /// <returns>Return a PacketType enumeration (MRP, SRP, SIP, CEP or timeout). If timeout is returned, no AMASP packet was found.</returns>
    PacketType readPacket(int &deviceID, byte message[], int &codeLength);

  private:
    HardwareSerial *masterCom = NULL;
};

//Serial Slave class
class AMASPSerialSlave
{
  public:
    AMASPSerialSlave();
    /// <summary>
    /// Initializes the slave connecting it to the serial link.
    /// </summary>
    /// <param name="serial">Serial communication object. </param>
    void begin(HardwareSerial &serial);
    /// <summary>
    /// Finalizes the slave disconnect it from the serial link.
    /// </summary>
    void end();
    //Send a SRP packet
    /// <summary>
    /// Send a SRP (Slave Response Packet).
    /// </summary>
    /// <param name="deviceID">Device identification. </param>
    /// <param name="message">Message to be send from the associated device.</param>
    /// <param name="msgLength">Message length in bytes.</param>
    void sendResponse(int deviceID, byte message[], int msgLength);
    /// <summary>
    /// Send a SIP (Slave Interruption Packet).
    /// </summary>
    /// <param name="deviceID">Device identification. </param>
    /// <param name="errorCode">The interruption code.</param>
    void sendInterruption(int deviceID, int code);
    /// <summary>
    /// Send a CEP (Communication Error Packet).
    /// </summary>
    /// <param name="deviceID">Device identification. </param>
    /// <param name="errorCode">The communication error code.</param>
    void sendError(int Device, int code);
    /// <summary>
    /// Read the incoming AMASP packet.
    /// </summary>
    /// <param name="deviceID">Device identification. </param>
    /// <param name="message">Message read from the associated device.</param>
    /// <param name="msgLength">Message length in bytes.</param>
    /// <returns>Return a PacketType enumeration (MRP, SRP, SIP, CEP or timeout). If timeout is returned, no AMASP packet was found.</returns>
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
