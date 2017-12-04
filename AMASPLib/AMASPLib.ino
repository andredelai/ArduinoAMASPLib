#include "AMASP.h"
#include "Arduino.h"

//AMASPSerialMaster com;
void setup() {
  // put your setup code here, to run once:
  long device;
  int codeLength;
  Serial.begin(9600);
  //com.begin(&Serial);
  byte data[]{'F','0','0','0'};
  //delay(2000);

  device = asciiHexToInt(data, 4);
  //Serial.println();
  Serial.println(device);
  //com.sendRequisition(0x0FDA, data, 4);
  //com.readPacket(&device, data, &codeLength);
}


void loop() {
  // put your main code here, to run repeatedly:

}
