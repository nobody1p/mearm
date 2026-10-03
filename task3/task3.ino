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

joystick_control(){
if (analogRead(A0) < 400) {
    if (base.read() > 0) {
      baseRos = base.read();
      base.write((base.read() - 1));
      delay(runSpeed);

    }

  } else if (analogRead(A0) > 600) {
    if (base.read() < 180) {
      baseRos = base.read();
      base.write((base.read() + 1));
      delay(runSpeed);

    }
  }
  if (analogRead(A1) < 400) {
    if (left.read() > 0) {
      leftRos = left.read();
      left.write((left.read() - 1));
      delay(runSpeed);

    }

  } else if (analogRead(A1) > 600) {
    if (left.read() < 180) {
      leftRos = left.read();
      left.write((left.read() + 1));
      delay(runSpeed);

    }
  }
  if (analogRead(A2) < 400) {
    if (right.read() > 0) {
      rightRos = right.read();
      right.write((right.read() - 1));
      delay(runSpeed);

    }

  } else if (analogRead(A2) > 600) {
    if (right.read() < 180) {
      rightRos = right.read();
      right.write((right.read() + 1));
      delay(runSpeed);

    }
  }
  if (analogRead(A3) < 400) {
    if (claw.read() > 0) {
      clawRos = claw.read();
      claw.write((claw.read() - 1));
      delay(runSpeed);

    }

  } else if (analogRead(A3) > 600) {
    if (claw.read() < 180) {
      clawRos = claw.read();
      claw.write((claw.read() + 1));
      delay(runSpeed);

    }
  }//摇杆操控函数

  Serial.print(analogRead(A0));
  Serial.print(",");
  Serial.print(analogRead(A1));
  Serial.print(",");
  Serial.print(digitalRead(5));
  Serial.print("--");
  Serial.print(analogRead(A2));
  Serial.print(",");
  Serial.print(analogRead(A3));
  Serial.print(",");
  Serial.println(digitalRead(4));
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
pinMode(D1,INPUT_PULLUP)
pinMode(D2,INPUT_PULLUP)
pinMode(D5,INPUT_PULLUP)
pinMode(D6,INPUT_PULLUP)
clawRos=70;
  delay(300); 
}

void loop() {
  // put your main code here, to run repeatedly:
  joystick_control()
  tenet = digitalRead(D1);
  record = digitalRead(D2);
  play = digitalRead(D5);
  back = digitalRead(D6);
  if(tenet == 0){
    
  }
  if(record == 0){
    base_current_angle = analogRead(A0);
    left_current_angle = analogRead(A1);
    right_current_angle = analogRead(A2);
    claw_current_angle = analogRead(A3);
    for(int i=0;;i++){
    joystick_control()
int records1[i]=analogRead(A0);
int records2[i]=analogRead(A1);
int records3[i]=analogRead(A2);
int records4[i]=analogRead(A3);
    delay(300);
    if(record == 0){
      break;
    }
  }
  if(paly == 0){
    base.write(base_current_angle);
    left.write(left_current_angle);
    right.write(right_current_angle);
    claw.write(claw_current_angle);
    for(int i=0;;i++){
    base.write(record1[i]);
    left.write(record2[i]);
    right.write(record3[i]);
    claw.write(record4[i]);
    }
  }
  if(back == 0){
    base.write(90); 
    left.write(90); 
    right.write(90);
    claw.write(70);
  }
}
