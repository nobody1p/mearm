#include <Servo.h>
Servo base;
Servo right;
Servo left;
Servo claw;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
base.attach(D9);
left.attach(D8);
right.attach(D7);
claw.attach(D6);
}

void loop() {
  // put your main code here, to run repeatedly:

}
