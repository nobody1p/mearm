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
int base_current_angle;
int left_current_angle;
int right_current_angle;
int claw_current_angle;
char serialCmd;
char servoCmd;
int b_fromRos;
int l_fromRos;
int r_fromRos;
int c_fromRos;
int b_toRos;
int l_toRos;
int r_toRos;
int c_toRos;
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
clawRos=70;
delay(300); 
pinMode(5, INPUT_PULLUP);
pinMode(4, INPUT_PULLUP);
delay(300); 
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("请输入要使用的模块");
  Serial.println("1:");
  Serial.println("2:");
  Serial.println("3:");
  Serial.println("4:");
  Serial.println("若要退出使用中的模块,请输入0");
if(Serial.avaluable())
serialCmd = Serial.read();
switch(SerialCmd){
  case 1:
  if(Serial.available()){
    servoCmd = Serial.read();
  if(servoCmd==0){
    break;
  }
  }
  case 2:
  if(Serial.available()){
    servoCmd = Serial.read();
  if(servoCmd==0){
    break;
  }
  }
  case 3:
  if(Serial.available()){
    servoCmd = Serial.read();
  if(servoCmd==0){
    break;
  }
  }
  case 4:if(Serial.available()){
    servoCmd = Serial.read();
  if(servoCmd==0){
    break;
  }
  }
  default:
  println("unknown command");
  break;
}
}
