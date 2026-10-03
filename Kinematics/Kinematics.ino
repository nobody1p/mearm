#include <Servo.h>
#include <math.h>
char command;
int runSpeed;
int clawRos;
int temp;
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
void Kinematics(float x,float y,float z){
rad1 = atan2(y,x);
r = sqrt(x*x + y*y);
d = sqrt(r*r+z*z);
rad3 = PI-acos((L1*L1+L2*L2-d*d)/(2*L1*L2));
rad2 = atan2(z,r)-atan2(L2*sin(rad3),L1+L2*cos(rad3));
angle1 = rad1*(180/PI);
angle2 = rad2*(180/PI);
angle3 = rad3*(180/PI);
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
    x=Serial.parseFloat();
    y=Serial.parseFloat();
    z=Serial.parseFloat();
    Serial.print("x:");
    Serial.println(x);
    Serial.print("y:");
    Serial.println(y);
    Serial.print("z:");
    Serial.println(z);
    Kinematics(x,y,z);
    Serial.println(angle1);
    Serial.println(angle2);
    Serial.println(angle3);
    while (Serial.available()) {
  Serial.read();
}
delay(500);
base.write(angle1); 
left.write(angle3); 
right.write(-angle2);
}
}
