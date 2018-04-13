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

///<sumary>
///AMASP Packet types returned by readPacket function.
///MRP - Master Request Packet
///SRP - Slave Response Packet
///SIP - Slave Interrupt Packet
///CEP - Communication Error Packet
///Timeout - No packet found
/// </summary>
enum PacketType {MRP = 0, SRP, SIP, CEP, Timeout};

//Serial Master class
class AMASPSerialMaster
{
  public:
    /// <summary>
    ///Constructor
    /// </summary>
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
    /// <summary>
    ///Constructor
    /// </summary>
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
    /// <returns>Returns a PacketType enumeration (MRP, SRP, SIP, CEP or timeout). If timeout is returned, no AMASP packet was found.</returns>
    PacketType readPacket(int &deviceID, byte message[], int &codeLength);

  private:
    HardwareSerial *slaveCom = NULL;
};

//Auxiliary functions***********


/// <summary>
/// Decimal int to ASCII hex conversion
/// </summary>
/// <param name="value">Decimal int value. </param>
/// <param name="hex">Hexadecimal 4 chars value (output).</param>
void intToASCIIHex(int value, char hex[]);

/// <summary>
/// ASCII Hex to a long decimal conversion
/// </summary>
/// <param name="hex">Hexadecimal value. </param>
/// <param name="length">Hexadecimal caracters length.</param>
/// <returns>Returns the result of the conversion or -1 in a case of invalid number</returns>
long asciiHexToInt(char hex[], int length);

/// <summary>
/// Longitudinal Redundancy Check function (16 bits)
/// </summary>
/// <param name="data">Data array to the calculate the LRC. </param>
/// <param name="dataLength">Data length.</param>
/// <returns>Returns the calculated LRC value.</returns>
long LRC(byte* data, int dataLength);

/// <summary>
/// Cyclic Redundancy Check function (16 bits)
/// </summary>
/// <param name="data">Data array to the calculate the LRC. </param>
/// <param name="dataLength">Data length.</param>
/// <returns>Returns the calculated LRC value.</returns>
short CRC16SerialModbus(byte* data, int dataLength);

#endif
