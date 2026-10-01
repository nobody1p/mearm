#include <Servo.h>
char command2;
int runSpeed;
int clawRos;
const int rosMin=0;
const int baseMax=100;
const int rightMax=100;
const int leftMax=100;
const int clawMax=180;
const int runSpeedMax=100;
Servo base,right,left,claw;

void task_2_clasp_and_move(){
command2 = Serial.read();
switch(command2){
  case 'A':
  case 'a':
   Serial.println("执行任务1");
  case 'B':
  case 'b':
   Serial.println("执行任务2");
  case 'C':
  case 'c':
   Serial.println("执行任务3");
}
}

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
}

void loop() {
  // put your main code here, to run repeatedly:

}
