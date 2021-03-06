/*
  AMASP.h - AMASP (ASCII MAster Slave Protocol) library for Arduino (version 1.0.0).
  Created by Andre Luiz Delai, Mar 11, 2018.
  Copyright (c) 2019 Andre L. Delai.  All right reserved.

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
#define MSGMAXSIZE 64

///<sumary>
///AMASP Packet types returned by readPacket function.
///MRP - Master Request Packet
///SRP - Slave Response Packet
///SIP - Slave Interrupt Packet
///CEP - Communication Error Packet
///Timeout - No packet found
/// </summary>
enum PacketType {MRP = 0, SRP, SIP, CEP, Timeout};

///<sumary>
///AMASP supported algorithms to error checking.
///</summary>
enum ErrorCheck {none = 0, XOR8, checksum16, LRC16, fletcher16, CRC16};

//Serial Master class
class AMASPSerialMaster
{
  public:
    /// <summary>
    /// Initializes the master connecting it to the serial port.
    /// </summary>
    /// <param name="serial">Serial communication object (input). </param>
    void begin(HardwareSerial &serial);

    /// <summary>
    /// Finalizes the master disconnect it from the serial port.
    /// </summary>
    void end();

    //Send a MRP packet
    /// <summary>
    /// Send a MRP (Master Request Packet).
    /// </summary>
    /// <param name="deviceID">The device identification (input). </param>
    /// <param name="message">The message to be send to the associated device (input).</param>
    /// <param name="msgLength">Message length in bytes (input).</param>
    /// <returns>Returns the generated error check data.</returns>
    int sendRequest(int deviceID, byte message[], int msgLength);

    /// <summary>
    /// Send a CEP (Communication Error Packet).
    /// </summary>
    /// <param name="deviceID">The device identification (input). </param>
    /// <param name="errorCode">The communication error code (input).</param>
    /// <returns>Returns the generated error check data.</returns>
    int sendError(int device, int errorCode);

    /// <summary>
    /// Read the incoming AMASP packet.
    /// </summary>
    /// <param name="deviceID">The device identification (output). </param>
    /// <param name="message">The message read from the associated device (output).</param>
    /// <param name="msgLength">The Message length in bytes (output).</param>
    /// <returns>Returns a PacketType enumeration (MRP, SRP, SIP, CEP or timeout). If timeout is returned, no valid AMASP packet was found.</returns>
    PacketType readPacket(int &deviceID, byte message[], int &codeLength);

    /// <summary>
    /// Read the incoming AMASP packet.
    /// </summary>
    /// <param name="deviceID">The device identification (output). </param>
    /// <param name="message">The message read from the associated device (output).</param>
    /// <param name="msgLength">The Message length in bytes (output).</param>
    /// <param name="eca">The error Check Algorithm used by the packet (output).</param>
    /// <param name="ecd">The error Check Data of the packet (output).</param>
    /// <returns>Returns a PacketType enumeration (MRP, SRP, SIP, CEP or timeout). If timeout is returned, no valid AMASP packet was found.</returns>
    PacketType readPacket(int &deviceID, byte message[], int &codeLength, ErrorCheck &eca, int &ecd);

    /// <summary>
    /// Sets the ECA (Error Check Algorithm).
    /// </summary>
    /// <param name="eca">Error Check Algorithm. </param>
    void setErrorCheck(ErrorCheck eca);


  private:
    HardwareSerial *masterCom = NULL;
    char errorCheckAlg = 0;
};

//Serial Slave class
class AMASPSerialSlave
{
  public:
    /// <summary>
    /// Initializes the slave connecting it to the serial port.
    /// </summary>
    /// <param name="serial">Serial communication object (input). </param>
    void begin(HardwareSerial &serial);

    /// <summary>
    /// Finalizes the slave disconnect it from the serial port.
    /// </summary>
    void end();

    //Send a SRP packet
    /// <summary>
    /// Send a SRP (Slave Response Packet).
    /// </summary>
    /// <param name="deviceID">The device identification (input). </param>
    /// <param name="message">The message to be send from the associated device (input).</param>
    /// <param name="msgLength">The message length in bytes (input).</param>
    /// <returns>Returns the generated error check data.</returns>
    int sendResponse(int deviceID, byte message[], int msgLength);

    /// <summary>
    /// Send a SIP (Slave Interruption Packet).
    /// </summary>
    /// <param name="deviceID">The device identification (input). </param>
    /// <param name="errorCode">The interruption code (input).</param>
    /// <returns>Returns the generated error check data.</returns>
    int sendInterruption(int deviceID, int code);
    
    /// <summary>
    /// Send a CEP (Communication Error Packet).
    /// </summary>
    /// <param name="deviceID">The device identification (input). </param>
    /// <param name="errorCode">The communication error code (input).</param>
    /// <returns>Returns the generated error check data.</returns>
    int sendError(int Device, int code);
    
    /// <summary>
    /// Read the incoming AMASP packet.
    /// </summary>
    /// <param name="deviceID">The device identification (output). </param>
    /// <param name="message">The message read from the associated device (output).</param>
    /// <param name="msgLength">The Message length in bytes (output).</param>
    /// <returns>Returns a PacketType enumeration (MRP, SRP, SIP, CEP or timeout). If timeout is returned, no valid AMASP packet was found.</returns>
    PacketType readPacket(int &deviceID, byte message[], int &codeLength);

    /// <summary>
    /// Read the incoming AMASP packet.
    /// </summary>
    /// <param name="deviceID">The device identification (output). </param>
    /// <param name="message">The message read from the associated device (output).</param>
    /// <param name="msgLength">The Message length in bytes (output).</param>
    /// <param name="eca">The error Check Algorithm used by the packet (output).</param>
    /// <param name="ecd">The error Check Data of the packet (output).</param>
    /// <returns>Returns a PacketType enumeration (MRP, SRP, SIP, CEP or timeout). If timeout is returned, no valid AMASP packet was found.</returns>
    PacketType readPacket(int &deviceID, byte message[], int &codeLength, ErrorCheck &eca, int &ecd);

    

    /// <summary>
    /// Sets the ECA (Error Check Algorithm).
    /// </summary>
    /// <param name="eca">Error Check Algorithm (input). </param>
    void setErrorCheck(ErrorCheck eca);

  private:
    HardwareSerial *slaveCom = NULL;
    char errorCheckAlg = 0;
};

//Auxiliary functions***********

/// <summary>
/// Decimal int to ASCII hex conversion
/// </summary>
/// <param name="value">Decimal int value (input). </param>
/// <param name="hex">Hexadecimal 4 chars value (output).</param>
void intToASCIIHex(int value, char hex[]);

/// <summary>
/// ASCII Hex to a long decimal conversion
/// </summary>
/// <param name="hex">Hexadecimal value (input). </param>
/// <param name="length">Hexadecimal caracters length (input).</param>
/// <returns>Returns the result of the conversion or -1 in a case of invalid number</returns>
long asciiHexToInt(char hex[], int length);

/// <summary>
/// Error checking function (16 bits)
/// </summary>
/// <param name="data">Data array to the calculate the error checking (input). </param>
/// <param name="dataLength">Data length (input).</param>
/// <param name="errorCheckAlg">The choosed error check algorithm (input).</param>
/// <returns>Returns the calculated value (input).</returns>
uint16_t errorCheck(byte *data, int dataLength, int errorCheckAlg);


#endif
