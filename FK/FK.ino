#include <Servo.h>
#include <math.h>

// ========== 舵机 ==========
Servo base, right, left, claw;

// ========== 示教记录 ==========
int flag[4][5];
int record1[100];
int record2[100];
int record3[100];
int record4[100];
int temp = -1;

// ========== 状态与命令 ==========
char cmd;
char serialCmd;
char servoCmd;
bool isPaused = false;

// ========== 速度与控制 ==========
int runSpeed = 15;
int runSpeedMax = 100;
int rosMin = 0;
const int baseMax = 100;
const int rightMax = 100;
const int leftMax = 100;
const int clawMax = 180;

// ========== 功能标志 ==========
bool tenet = false;
bool record = false;
bool play = false;
bool back = false;
bool taskA = false;
bool taskAA = false;
bool taskAAA = false;
bool backA = false;
bool taskB = false;
bool taskBB = false;
bool taskBBB = false;
bool backB = false;
bool taskC = false;
bool taskCC = false;
bool taskCCC = false;
bool backC = false;

// ========== 当前角度（各种命名都保留） ==========
int base_current_angle;
int left_current_angle;
int right_current_angle;
int claw_current_angle;

// ========== ROS相关变量（全部保留） ==========
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

// ========== 运动学变量 ==========
float x, y, z, d, r, h1, h2;
float angle1, angle2, angle3;
float rad1, rad2, rad3;
float L1 = 80;
float L2 = 80;
float L3 = 20;
float H = 60;
float base_angle,left_angle,right_angle;

// ========== 介质变量 ==========
int i = 0;
bool recordStart = false;
bool circle1 = false;
bool circle2 = false;

//======== 笛卡尔空间运动规划 =========
float step_size = 0.5;
int N;
float lenth;
float dx;
float dy;
float dz;
float x_now;
float y_now;
float z_now;
bool next = false;
bool point = false;
int n=0;
float x1,x2,y1,y2,z1,z2;


void ForwardKinematics() {
  angle1 = base.read() - 90;
  angle2 = 150 - right.read();
  angle3 = angle2 - left.read() + 60;

  rad1 = angle1 * (PI / 180);
  rad2 = angle2 * (PI / 180);
  rad3 = angle3 * (PI / 180);

  r = L1 * cos(rad2) + L2 * cos(rad2 - rad3);   
  z = L1 * sin(rad2) + L2 * sin(rad2 - rad3);

  r = r + L3;
  z = z + H;   
  x = r * cos(rad1);
  y = r * sin(rad1);
}

void Kinematics(float x,float y,float z){
r = sqrt(x*x + y*y) - L3;
z = z - H;
d = sqrt(r*r+z*z);

rad1 = atan2(y,x);
rad3 = PI-acos((L1*L1+L2*L2-d*d)/(2*L1*L2));
rad2 = atan2(z,r)+atan2(L2*sin(rad3),L1+L2*cos(rad3));

angle1 = rad1*(180/PI);
angle2 = rad2*(180/PI);
angle3 = rad3*(180/PI);

base_angle = 90 + angle1;
left_angle = angle2 - angle3 + 60;
right_angle = 150 - angle2;
}

void straight_line_prepare(float x1,float x2,float y1,float y2,float z1,float z2){
  lenth = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
  N = lenth / step_size;
  if (N < 1) N = 1;
  dx = (x2 - x1) / N;
  dy = (y2 - y1) / N;
  dz = (z2 - z1) / N;
  x_now = x1;
  y_now = y1;
  z_now = z1;
  n=0;
  next = !next;
}

void straight_line(){
  if(next == true){
    x_now = x_now + dx;
    y_now = y_now + dy;
    z_now = z_now + dz;
    n++;
    if(n >= N){
      next = !next;
      n = 0;
    }
    Kinematics(x_now,y_now,z_now);
    base.write(base_angle);
    left.write(left_angle);
    right.write(right_angle);
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
if(Serial.available()){
  cmd = Serial.read();
}
if(cmd == 'q'){
  x1 = Serial.parseFloat();
  y1 = Serial.parseFloat();
  z1 = Serial.parseFloat();
  x2 = Serial.parseFloat();
  y2 = Serial.parseFloat();
  z2 = Serial.parseFloat();
  Kinematics(x1,y1,z1);
  base.write(base_angle);
  left.write(left_angle);
  right.write(right_angle);
  delay(500);
  straight_line_prepare(x1,x2,y1,y2,z1,z2);
  cmd = 'sleep';
}
straight_line();
}