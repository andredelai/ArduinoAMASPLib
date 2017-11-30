#include "AMASP.h"
#include "Arduino.h"

AMASPSerialMaster com;
void setup() {
  // put your setup code here, to run once:
  int device;
  int codeLength;
  Serial.begin(9600);
  com.begin(&Serial);
  byte data[]{'X','X','X','X'};
  delay(2000);
  com.sendRequisition(0x0FDA, data, 4);
  com.readPacket(&device, data, &codeLength);
  
}


void loop() {
  // put your main code here, to run repeatedly:

}
