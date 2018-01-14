AMASP Arduino Library

Version 1.0

This library implements the AMASP (ASCII Master/Slave Protocol) for Arduino boards, a simple way to exchange messagens between two computers using serial communication.

AMASP is free and uses four different packets:

MASTER -> SLAVE:

 MRP - Master Request Packet
 CEP - Communication Error Packet

SLAVE -> MASTER:

 SRP - Slave Response Packet 
 SIP - Slave Interruption Packet
 CEP - Communication Error Packet

The protocol is transnparent to the user that only needs to use the AMASP Arduino Library functions to implement his own applications. See the examples codes!

The library is always in test and improvements, if you have any problem using it, please send a mail to the author (spanish, portuguese or english) adelai@gmail.com.

Contributors will be welcome!

Wants to design an AMASP library to other platforms? Be my guest!


Author:

Andre L. Delai
adelai@gmail.com

Enjoy :)