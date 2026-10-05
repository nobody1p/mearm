#include <Servo.h>
#include <math.h>
char command;
int runSpeed;
int clawRos;
int temp;
int tenet;
int record;
int play;
int back;
float x,y,z,d,r,h1,h2,angle1,angle2,angle3,rad1,rad2,rad3;
float L1=63.5;
float L2=112;
const int rosMin=0;
const int baseMax=100;
const int rightMax=100;
const int leftMax=100;
const int clawMax=180;
const int runSpeedMax=100;
Servo base,right,left,claw;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
base.attach(9);
left.attach(8);
right.attach(7);
claw.attach(6);
base.write(90); 
left.write(90); 
right.write(90);
claw.write(70);
pinMode(D1,INPUT_PULLUP)
pinMode(D2,INPUT_PULLUP)
pinMode(D5,INPUT_PULLUP)
pinMode(D6,INPUT_PULLUP)
clawRos=70;
  delay(300); 
}

void loop() {
  // put your main code here, to run repeatedly:
if(Serial.avaluable()){
  switch(temp){
    case A:
    case a:
    servo.write();
    case B:
    case b:
    servo.write();
    case C:
    case c:
    servo.write();
  }
}
}
