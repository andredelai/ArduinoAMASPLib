#include "AMASP.h"

AMASPSerialMaster com;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  com.begin(&Serial);
  byte data[]{'X','X','X','X'};
  while(1)
  {
    delay(2000);
    com.sendRequisition(0x0FDA, data, 4);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
