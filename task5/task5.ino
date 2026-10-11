#include <Servo.h>
#include <math.h>
int flag[4][5];
int i=0;
char command;
bool isPaused = false;
int runSpeed;
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

int baseMove(int b_fromRos,int b_toRos){
  b_fromRos+=(b_fromRos-b_toRos)*0.15;
    base.write(b_fromRos);
    delay(runSpeed);
}

int leftMove(int l_fromRos,int l_toRos){
  l_fromRos+=(l_fromRos-l_toRos)*0.15;
    left.write(l_fromRos);
    delay(runSpeed);
}

int rightMove(int r_fromRos,int r_toRos){
  r_fromRos+=(r_fromRos-r_toRos)*0.15;
    right.write(r_fromRos);
    delay(runSpeed);
}

int clawMove(int c_fromRos,int c_toRos){
  c_fromRos+=(c_fromRos-c_toRos)*0.15;
    claw.write(c_fromRos);
    delay(runSpeed);
}

void pause(){
  if(command=='pause'){
    isPaused = true;
}
}

void cancel(){
  if(command=='cancel'){
    b_fromRos=base.read();
    l_fromRos=left.read();
    r_fromRos=right.read();
    c_fromRos=claw.read();
    b_toRos=90;
    l_toRos=90;
    r_toRos=90;
    c_toRos=70;
    baseMove(b_fromRos,b_toRos);
    leftMove(f_fromRos,f_toRos);
    rightMove(r_fromRos,r_toRos);
    clawMove(c_fromRos,c_toRos);
  }
}

void start(){
  if(command=='start'){
    isPaused = false;
}
}

void pointRecord(){
  if(command=='flag'){
    flag[0][i]=base.read();
    flag[1][i]=left.read();
    flag[2][i]=right.read();
    flag[3][i]=claw.read();
    i++;
    command=sleep;
    if(i>=3){
      i=0;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Wire.begin(8);
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
  start();
  pause();
  cancel();
  pointRecord();
if(isPaused){
  return;
}

}
