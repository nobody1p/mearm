#include <Servo.h>
#include <math.h>
char command;
int runSpeed =10;
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

int baseMove(){
b_fromRos+=(b_toRos-b_fromRos)*0.15;
    base.write(b_fromRos);
    delay(runSpeed);
}

int leftMove(){
  l_fromRos+=(l_toRos-l_fromRos)*0.15;
    left.write(l_fromRos);
    delay(runSpeed);
}

int rightMove(){
  r_fromRos+=(r_toRos-r_fromRos)*0.15;
    right.write(r_fromRos);
    delay(runSpeed);
}

int clawMove(){
  c_fromRos+=(c_toRos-c_fromRos)*0.15;
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
    case 'a':
    b_toRos=101;
    l_toRos=23;
    r_toRos=122;
    c_toRos=100;
    b_fromRos=90;
    l_fromRos=90;
    r_fromRos=90;
    c_fromRos=70;
    while((b_toRos-b_fromRos)>=1){
    baseMove();
    leftMove();
    rightMove();
    }
    while((c_toRos-c_fromRos)>=1){
    clawMove();
  }
    break;
    case 'B':
    case 'b':
    b_toRos=101;
    l_toRos=23;
    r_toRos=122;
    c_toRos=100;
    b_fromRos=90;
    l_fromRos=90;
    r_fromRos=90;
    c_fromRos=70;
    
    break;
    case 'C':
    case 'c':
    b_toRos=101;
    l_toRos=23;
    r_toRos=122;
    c_toRos=100;
    b_fromRos=90;
    l_fromRos=90;
    r_fromRos=90;
    c_fromRos=70;
    
  }
}
}
