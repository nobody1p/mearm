#include <Servo.h>
#include <math.h>
char cmd;
int runSpeed =30;
int temp;
int tenet;
int record;
int play;
int back;
int base_current_angle = 90;
int left_current_angle = 90;
int right_current_angle = 90;
int claw_current_angle = 70;
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
int clawRos;
int baseRos;
int rightRos;
int leftRos;
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

bool taskA = false;
bool taskAA = false;
bool taskAAA = false;
bool backA = false;

int baseMove(){
b_fromRos+=(b_toRos-b_fromRos)*0.25;
    base.write(b_fromRos);
    delay(runSpeed);
}

int leftMove(){
  l_fromRos+=(l_toRos-l_fromRos)*0.25;
    left.write(l_fromRos);
    delay(runSpeed);
}

int rightMove(){
  r_fromRos+=(r_toRos-r_fromRos)*0.25;
    right.write(r_fromRos);
    delay(runSpeed);
}

int clawMove(){
  c_fromRos+=(c_toRos-c_fromRos)*0.25;
    claw.write(c_fromRos);
    delay(runSpeed);
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
if(Serial.available()){
  cmd=Serial.read();
  switch(cmd){
   case 'A':
   taskA=true;
    break;
    case 'B':
    case 'b':
    break;
    case 'C':
    case 'c':
    break;
  }
}
void Amove(){
if(taskA == true){
    if(base_current_angle < 150) base_current_angle += 5;
    if(left_current_angle > 0)   left_current_angle -= 5;
    if(right_current_angle < 100) right_current_angle += 5;

    if(base_current_angle > 150) base_current_angle = 150;
    if(left_current_angle < 0)   left_current_angle = 0;
    if(right_current_angle > 100) right_current_angle = 100;
    left.write(left_current_angle);
    right.write(right_current_angle);
    base.write(base_current_angle);
    delay(runSpeed);
    if((base_current_angle==150)&&(left_current_angle==0)&&(right_current_angle==100)){
      claw.write(120);
      taskA=false;
      taskAA=true;
      delay(500);
    }
}
    if(taskAA == true){
     if(base_current_angle > 110) base_current_angle -= 5;  
    if(left_current_angle < 90)  left_current_angle += 5;   
    if(right_current_angle > 90) right_current_angle -= 5; 

    if(base_current_angle < 110) base_current_angle = 110;
    if(left_current_angle > 90)  left_current_angle = 90;
    if(right_current_angle < 90) right_current_angle = 90;
      left.write(left_current_angle);
      right.write(right_current_angle);
      base.write(base_current_angle);
      delay(runSpeed);
      if((base_current_angle==110)&&(left_current_angle==90)&&(right_current_angle==90)){
        taskAA=false;
        taskAAA=true;
        delay(500);
      }
    }
    if(taskAAA == true){
    if(left_current_angle > 0)   left_current_angle -= 5;
    if(right_current_angle < 100) right_current_angle += 5;

    if(left_current_angle < 0)   left_current_angle = 0;
    if(right_current_angle > 100) right_current_angle = 100;
      left.write(left_current_angle);
      right.write(right_current_angle);
      delay(runSpeed);
      if((left_current_angle==0)&&(right_current_angle==100)){
        claw.write(70);
        taskAAA=false;
        backA=true;
        delay(500);
      }
    }
    if(backA==true){
    if(base_current_angle > 90) base_current_angle -= 5;   
    if(left_current_angle < 90)  left_current_angle += 5;   
    if(right_current_angle > 90) right_current_angle -= 5; 

    if(base_current_angle < 90) base_current_angle = 90;
    if(left_current_angle > 90)  left_current_angle = 90;
    if(right_current_angle < 90) right_current_angle = 90;
      left.write(left_current_angle);
      right.write(right_current_angle);
      base.write(base_current_angle);
      delay(runSpeed);
      if((base_current_angle==90)&&(left_current_angle==90)&&(right_current_angle==90)){
        backA=false;
        delay(500);
      }
    }
}
}
