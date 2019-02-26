//  created december 2017
//  by Andre L. Delai <adelai@gmail.com>

// Master controls the Arduino (slave) board to turn on or turn off the onboard led.

// Send 'H' as message to turn on and 'L' to turn off

//  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
//  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
//  the correct LED pin independent of which board is used.
//  If you want to know what pin the on-board LED is connected to on your Arduino
//  model, check the Technical Specs of your board at:
//  https://www.arduino.cc/en/Main/Products

//	This example code is in the public domain.

#include <AMASP.h>

#define LED 1 //Define the LED as device 1 in the protocol.

AMASPSerialSlave slave; //AMASP slave instance
int device, codeLength; 
byte msg[1]; 
PacketType type;

void setup() {
  // initialize Arduino serial port
  Serial.begin(9600);
  //Clear the serial
  Serial.flush();
  // connect the slave to the serial port
  slave.begin(Serial);

  //No error checking
  slave.SetErrorCheck(none);
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) //something arrived in serial buffer?
  {
    type = slave.readPacket(device, msg, codeLength); //Checking if is a amasp packet
    if (type == MRP) //packet from master received
    {
      switch (device)//checking the device
      {
        case LED:
          if (msg[0] == 'H')
          {
            //LED on
            digitalWrite(LED_BUILTIN, HIGH);
            slave.sendError(LED, 00); //Message received ok
          }
          else if (msg[0] == 'L')
          {
            //LED off
            digitalWrite(LED_BUILTIN, LOW);
            slave.sendError(LED, 00); //Message received ok
          }
          else
          {
            slave.sendError(LED, 03); //Unrecognized message
          }
          break;
        default:
          slave.sendError(device, 02); //Invalid device
          break;
      }
    }
  }
}
