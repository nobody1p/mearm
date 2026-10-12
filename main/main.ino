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
bool backB = false;
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

// ========== 介质变量 ==========
int i = 0;
bool recordStart = false;
bool circle1 = false;
bool circle2 = false;

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
 baseRos = Serial.parseInt();
 base.write(baseRos);
 break;

case 'l':
 leftRos = Serial.parseInt();
 left.write(leftRos);
 break;

case 'r':
 rightRos = Serial.parseInt();
 right.write(rightRos);
 break;

case 'c':
 clawRos = Serial.parseInt();
 claw.write(clawRos);
 break;
}
}

// ========= task2 ABC移动函数 =========
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
    taskA=!taskA;
}

void Bmove(){
if(taskB == true){
    if(left_current_angle > 30)   left_current_angle -= 5;
    if(right_current_angle < 100) right_current_angle += 5;

    if(left_current_angle < 30)   left_current_angle = 30;
    if(right_current_angle > 100) right_current_angle = 100;
    left.write(left_current_angle);
    right.write(right_current_angle);
    base.write(base_current_angle);
    delay(runSpeed);
    if(((left_current_angle==30)&&(right_current_angle==100)){
      claw.write(120);
      taskB=false;
      taskBB=true;
      delay(500);
    }
}
    if(taskBB == true){
     if(base_current_angle > 60) base_current_angle -= 5;  
    if(left_current_angle < 90)  left_current_angle += 5;   
    if(right_current_angle > 90) right_current_angle -= 5; 

    if(base_current_angle < 60) base_current_angle = 60;
    if(left_current_angle > 90)  left_current_angle = 90;
    if(right_current_angle < 90) right_current_angle = 90;
      left.write(left_current_angle);
      right.write(right_current_angle);
      base.write(base_current_angle);
      delay(runSpeed);
      if((base_current_angle==60)&&(left_current_angle==90)&&(right_current_angle==90)){
        taskBB=false;
        taskBBB=true;
        delay(500);
      }
    }
    if(taskBBB == true){
    if(left_current_angle > 30)   left_current_angle -= 5;
    if(right_current_angle < 100) right_current_angle += 5;

    if(left_current_angle < 30)   left_current_angle = 30;
    if(right_current_angle > 100) right_current_angle = 100;
      left.write(left_current_angle);
      right.write(right_current_angle);
      delay(runSpeed);
      if((left_current_angle==30)&&(right_current_angle==100)){
        claw.write(70);
        taskBBB=false;
        backB=true;
        delay(500);
      }
    }
    if(backB==true){
    if(base_current_angle < 90) base_current_angle += 5;   
    if(left_current_angle < 90)  left_current_angle += 5;   
    if(right_current_angle > 90) right_current_angle -= 5; 

    if(base_current_angle > 90) base_current_angle = 90;
    if(left_current_angle > 90)  left_current_angle = 90;
    if(right_current_angle < 90) right_current_angle = 90;
      left.write(left_current_angle);
      right.write(right_current_angle);
      base.write(base_current_angle);
      delay(runSpeed);
      if((base_current_angle==90)&&(left_current_angle==90)&&(right_current_angle==90)){
        backB=false;
        delay(500);
      }
    }
    taskB=!taskB;
}

void Cmove(){
if(taskC == true){
    if(base_current_angle > 30) base_current_angle -= 5;
    if(left_current_angle > 0)   left_current_angle -= 5;
    if(right_current_angle < 100) right_current_angle += 5;

    if(base_current_angle > 30) base_current_angle = 30;
    if(left_current_angle < 0)   left_current_angle = 0;
    if(right_current_angle > 100) right_current_angle = 100;
    left.write(left_current_angle);
    right.write(right_current_angle);
    base.write(base_current_angle);
    delay(runSpeed);
    if((base_current_angle==30)&&(left_current_angle==0)&&(right_current_angle==100)){
      claw.write(120);
      taskC=false;
      taskCC=true;
      delay(500);
    }
}
    if(taskCC == true){
     if(base_current_angle < 100) base_current_angle += 5;  
    if(left_current_angle < 90)  left_current_angle += 5;   
    if(right_current_angle > 90) right_current_angle -= 5; 

    if(base_current_angle > 100) base_current_angle = 100;
    if(left_current_angle > 90)  left_current_angle = 90;
    if(right_current_angle < 90) right_current_angle = 90;
      left.write(left_current_angle);
      right.write(right_current_angle);
      base.write(base_current_angle);
      delay(runSpeed);
      if((base_current_angle==100)&&(left_current_angle==90)&&(right_current_angle==90)){
        taskCC=false;
        taskCCC=true;
        delay(500);
      }
    }
    if(taskCCC == true){
    if(left_current_angle > 0)   left_current_angle -= 5;
    if(right_current_angle < 100) right_current_angle += 5;

    if(left_current_angle < 0)   left_current_angle = 0;
    if(right_current_angle > 100) right_current_angle = 100;
      left.write(left_current_angle);
      right.write(right_current_angle);
      delay(runSpeed);
      if((left_current_angle==0)&&(right_current_angle==100)){
        claw.write(70);
        taskCCC=false;
        backC=true;
        delay(500);
      }
    }
    if(backC==true){
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
        backC=false;
        delay(500);
      }
    }
    task=!taskC;
}

// ========= 机械臂运动学反解函数 =========
void Kinematics(float x,float y,float z){
rad1 = atan2(y,x);
r = sqrt(x*x + y*y) - L3;
z = z - H;
d = sqrt(r*r+z*z);
rad3 = PI-acos((L1*L1+L2*L2-d*d)/(2*L1*L2));
rad2 = atan2(z,r)+atan2(L2*sin(rad3),L1+L2*cos(rad3));
angle1 = rad1*(180/PI);
angle2 = rad2*(180/PI);
angle3 = rad3*(180/PI);
}

// ========= 四舵机低通滤波运动函数 =========
int baseMove(int b_fromRos,int b_toRos){
  b_fromRos+=(b_fromRos-b_toRos)*0.25;
    base.write(b_fromRos);
    delay(runSpeed);
}

int leftMove(int l_fromRos,int l_toRos){
  l_fromRos+=(l_fromRos-l_toRos)*0.25;
    left.write(l_fromRos);
    delay(runSpeed);
}

int rightMove(int r_fromRos,int r_toRos){
  r_fromRos+=(r_fromRos-r_toRos)*0.25;
    right.write(r_fromRos);
    delay(runSpeed);
}

int clawMove(int c_fromRos,int c_toRos){
  c_fromRos+=(c_fromRos-c_toRos)*0.25;
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
    record=!record;
    recordStart=!recordStart;
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

  if(play == true){
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
    play=false;
  }

  if(back == true){
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
    back = false;
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
  tenet=!tenet;
  cmd = 'sleep';
}

if(tenet == true){
  if(circle1 == false){
    taskA = !taskA;
    circle1 = true;
  }
  else if(circle2 == false){
    taskB = !taskB;
    circle2 = true;
  }
  else{
    taskC = !taskC;
    circle1 = false;
    circle2 = false;
  }
  tenet = false;  
}

if(cmd == 'play'){
  play=!play;
  cmd = 'sleep';
}

if(cmd == 'record'){
  record=!record;
  cmd = 'sleep';
}

if(cmd == 'back'){
  back=!back;
  cmd = 'sleep';
}

if(cmd == 'A'){
  taskA=!taskA;
  cmd = 'sleep';
}

if(cmd == 'B'){
  taskB=!taskB;
  cmd = 'sleep';
}

if(cmd == 'C'){
  taskC=!taskC;
  cmd = 'sleep';
}


task_1_basicControl();

joystick_control();

moveA();
moveB();
moveC(;

task3();

task5();
}