#include "AMASP.h"
#include "Arduino.h"

AMASPSerialSlave com;
void setup() {
  // put your setup code here, to run once:
  int device;
  int codeLength;
  PacketType type;
  Serial.begin(9600);
  com.begin(&Serial);
  byte data[]{'F','E','6','6'};
  int msgSize;
  Serial.flush();

//  while(1)
//  {
//    Serial.print(asciiHexToInt(data, 4));
//    delay(2000);  
//  }
  
  
  while (1)
  {
    if (Serial.available() > 0)
    {
      type = com.readPacket(&device, data, &codeLength);
      if (type == MRP)
      {
        com.sendResponse(device, data, codeLength);
      }

      //      msgSize = Serial.available();
      //      Serial.readBytes(data, msgSize);
      //      Serial.write(data, msgSize);
      //      Serial.flush();
    }
  }

  //device = asciiHexToInt(data, 4);
  //Serial.println();
  Serial.println(device);
  //com.sendRequisition(0x0FDA, data, 4);
  //com.readPacket(&device, data, &codeLength);
}


void loop() {
  // put your main code here, to run repeatedly:

}
