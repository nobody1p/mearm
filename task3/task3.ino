#include <Servo.h>
#include <math.h>
int record1[100];
int record2[100];
int record3[100];
int record4[100];
int temp = -1;
char command;
int runSpeed = 15;
int clawRos;
int baseRos;
int rightRos;
int leftRos;
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

void joystick_control(){
if (analogRead(A0) < 400) {
    if (base.read() > 0) {
      baseRos = base.read();
      base.write((base.read() - 5));
      delay(runSpeed);

    }

  } else if (analogRead(A0) > 600) {
    if (base.read() < 180) {
      baseRos = base.read();
      base.write((base.read() + 5));
      delay(runSpeed);
    }
  }
  if (analogRead(A1) < 400) {
    if (left.read() > 0) {
      leftRos = left.read();
      left.write((left.read() - 5));
      delay(runSpeed);

    }

  } else if (analogRead(A1) > 600) {
    if (left.read() < 180) {
      leftRos = left.read();
      left.write((left.read() + 5));
      delay(runSpeed);

    }
  }
  if (analogRead(A2) < 400) {
    if (right.read() > 0) {
      rightRos = right.read();
      right.write((right.read() - 5));
      delay(runSpeed);

    }

  } else if (analogRead(A2) > 600) {
    if (right.read() < 180) {
      rightRos = right.read();
      right.write((right.read() + 5));
      delay(runSpeed);

    }
  }
  if (analogRead(A3) < 400) {
    if (claw.read() > 0) {
      clawRos = claw.read();
      claw.write((claw.read() - 5));
      delay(runSpeed);

    }

  } else if (analogRead(A3) > 600) {
    if (claw.read() < 180) {
      clawRos = claw.read();
      claw.write((claw.read() + 5));
      delay(runSpeed);

    }
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
  joystick_control();
  if(tenet == true){
      temp=temp+1;
  }
  if(record == true){
    base_current_angle = base.read();
    left_current_angle = left.read();
    right_current_angle = right.read();
    claw_current_angle = claw.read();
    !record;
    !recordStart;
    delay(300);
  }
if(recordStart == true){
joystick_control();
i++;
record1[i]=base.read();
record2[i]=left.read();
record3[i]=right.read();
record4[i]=claw.read();
    delay(100);
    if(record == true){
      temp=i;
      delay(300);
      break;
    }
  }
  }
  if(play == 0){
    base.write(base_current_angle);
    left.write(left_current_angle);
    right.write(right_current_angle);
    claw.write(claw_current_angle);
    delay(300);
    for(int i=0;i<=temp;i++){
    base.write(record1[i]);
    left.write(record2[i]);
    right.write(record3[i]);
    claw.write(record4[i]);
    delay(runSpeed);
    }
  }
  if(back == 0){
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