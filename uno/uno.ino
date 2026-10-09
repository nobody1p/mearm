#include <Wire.h>
char cmd;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Wire.begin(8);
}

void loop() {
  // put your main code here, to run repeatedly:
if(Wire.available()){
  cmd=Wire.read();
  Serial.println(cmd);
}
}
