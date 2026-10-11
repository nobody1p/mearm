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
float L1 = 63.5;
float L2 = 112;

// ========== 介质变量 ==========
int i = 0;
bool recordStart = false;

// ========= 摇杆操控函数 =========
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
// ========= 基础操控函数 =========
void task_1_basicControl(){
switch(cmd){
case'o':
case'O':
  clawRos-=10;
  if(clawRos<0){
    Serial.println("已经达到最大钳宽");
    clawRos+=10;
    break;
  }
claw.write(clawRos);
Serial.print("当前闭合度：");
Serial.println(clawRos);
break;

case's':
case'S':
  clawRos+=10;
  if(clawRos>clawMax){
    Serial.println("已经达到最小钳宽");
    clawRos-=10;
    break;
  }
claw.write(clawRos);
Serial.print("当前闭合度：");
Serial.println(clawRos);
break;

case'h':
case'H':
  runSpeed+=10;
  if(runSpeed>runSpeedMax){
  Serial.println("运行速度已经达到最大值");
  runSpeed-=10;
  break;
  }
  Serial.print("当前运行速度：");
  Serial.println(runSpeed);
break;

case'l':
case'L':
  runSpeed-=10;
  if(runSpeed<0){
  Serial.println("运行速度已经达到最小值");
  runSpeed+=10;
  break;
  }
  Serial.print("当前运行速度：");
  Serial.println(runSpeed);
  break;

case 'b':
case 'B':
 baseRos = Serial.parseInt();
 base.write(baseRos);
 break;

case 'e':
case 'E':
 leftRos = Serial.parseInt();
 left.write(leftRos);
 break;

case 'r':
case 'R':
 rightRos = Serial.parseInt();
 right.write(rightRos);
 break;

case 'j':
case 'J':
 clawRos = Serial.parseInt();
 claw.write(clawRos);
 break;
}
}

// ========= task2 ABC移动函数 =========
void task2(){
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

// ========= 机械臂运动学反解函数 =========
void Kinematics(float x,float y,float z){
rad1 = atan2(y,x);
r = sqrt(x*x + y*y);
d = sqrt(r*r+z*z);
rad3 = PI-acos((L1*L1+L2*L2-d*d)/(2*L1*L2));
rad2 = atan2(z,r)+atan2(L2*sin(rad3),L1+L2*cos(rad3));
angle1 = rad1*(180/PI);
angle2 = rad2*(180/PI);
angle3 = rad3*(180/PI);
}

// ========= 四舵机低通滤波运动函数 =========
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
// =========任务三 循环 记录 播放 回中 函数=========
void task3(){
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
      recordStart == false;
      delay(300);
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
    leftMove(l_fromRos,l_toRos);
    rightMove(r_fromRos,r_toRos);
    clawMove(c_fromRos,c_toRos);
  }
}

// =========任务五 暂停 继续 取消 记录示教点 函数=========
void pause(){
  if(cmd=='pause'){
    isPaused = true;
}
}

void cancel(){
  if(cmd=='cancel'){
    b_fromRos=base.read();
    l_fromRos=left.read();
    r_fromRos=right.read();
    c_fromRos=claw.read();
    b_toRos=90;
    l_toRos=90;
    r_toRos=90;
    c_toRos=70;
    baseMove(b_fromRos,b_toRos);
    leftMove(l_fromRos,l_toRos);
    rightMove(r_fromRos,r_toRos);
    clawMove(c_fromRos,c_toRos);
  }
}

void start(){
  if(cmd=='start'){
    isPaused = false;
}
}

void pointRecord(){
  if(cmd=='flag'){
    flag[0][i]=base.read();
    flag[1][i]=left.read();
    flag[2][i]=right.read();
    flag[3][i]=claw.read();
    i++;
    cmd='sleep';
    if(i>=3){
      i=0;
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
  if(Serial.available()){
    cmd = Serial.readString()
  }
  start();
  pause();
  cancel();
  pointRecord();
if(isPaused){
  return;
}
if(cmd == 'tenet'){
  !tenet;
  cmd = 'sleep';
}

if(cmd == 'play'){
  !play;
  cmd = 'sleep';
}

if(cmd == 'record'){
  !record;
  cmd = 'sleep';
}

if(cmd == 'back'){
  !back;
  cmd = 'sleep';
}

task_1_basicControl();

task2();

task3();
}