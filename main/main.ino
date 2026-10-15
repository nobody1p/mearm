#include <Servo.h>
#include <math.h>

// ========== 舵机 ==========
Servo base, right, left, claw;

// ========== 示教记录 ==========
float flag[4][5];
byte record1[300];
byte record2[300];
byte record3[300];
byte record4[300];
int temp = -1;

// ========== 状态与命令 ==========
char cmd = 0;
bool isPaused = false;

// ========== 速度与控制 ==========
int runSpeed = 20;
int runSpeedmin = 100;
int runSpeedMax = 10;
int rosMin = 0;
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
bool end = false;
bool preDraw = false;

// ========== 当前角度（各种命名都保留） ==========
int base_current_angle  = 90;
int left_current_angle  = 90;
int right_current_angle = 90;
int claw_current_angle  = 70;

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
int left_angle;
int right_angle;
int base_angle;

// ========== 运动学变量 ==========
float x, y, z, d, r
float angle1, angle2, angle3;
float rad1, rad2, rad3;
float L1 = 80;
float L2 = 80;
float L3 = 20;
float H = 60;

// ========== 介质变量 ==========
int i = 0;
bool recordStart = false;
int circle = 0;
int pointIndex = 0;    
int showIndex  = 0;    

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
int n = 0;
float x1,x2,y1,y2,z1,z2;

// ========== 时间片 ==========
unsigned long lastTick = 0;
unsigned long TICK_MS  = 2;

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
    cmd = 0;
    break;
  }
claw.write(clawRos);
Serial.print("当前闭合度：");
Serial.println(clawRos);
cmd = 0;
break;

case'S':
  clawRos+=10;
  if(clawRos>clawMax){
    Serial.println("已经达到最小钳宽");
    clawRos-=10;
    cmd = 0;
    break;
  }
claw.write(clawRos);
Serial.print("当前闭合度：");
Serial.println(clawRos);
cmd = 0;
break;

case'H':
  runSpeed-=10;
  if(runSpeed<runSpeedMax){
  Serial.println("运行速度已经达到最大值");
  runSpeed+=10;
  cmd = 0;
  break;
  }
  Serial.print("当前动作延迟：");
  Serial.println(runSpeed);
  cmd = 0;
  break;

case'L':
  runSpeed+=10;
  if(runSpeed>runSpeedmin){
  Serial.println("运行速度已经达到最小值");
  runSpeed-=10;
  cmd = 0;
  break;
  }
  Serial.print("当前动作延迟：");
  Serial.println(runSpeed);
  cmd = 0;
  break;

case 'b':
 baseRos = Serial.parseInt();
 base.write(baseRos);
 cmd = 0;
 break;

case 'l':
 leftRos = Serial.parseInt();
 left.write(leftRos);
 cmd = 0;
 break;

case 'r':
 rightRos = Serial.parseInt();
 right.write(rightRos);
 cmd = 0;
 break;

case 'c':
 clawRos = Serial.parseInt();
 claw.write(clawRos);
 cmd = 0;
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
      claw.write(150);
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
    if((left_current_angle==30)&&(right_current_angle==100)){
      claw.write(150);
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
      claw.write(150);
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
}

// ========= 舵机移动函数 =========
void writeServo(float b, float l, float rr){
  if (b  < 0)   b  = 0;
  if (b  > 180) b  = 180;
  if (l  < 0)   l  = 0;
  if (l  > 180) l  = 180;
  if (rr < 0)   rr = 0;
  if (rr > 180) rr = 180;
  base.write ((int)b);
  left.write ((int)l);
  right.write((int)rr);
}

// ========= 机械臂运动学反解函数 =========
void Kinematics(float x,float y,float z){
r = sqrt(x*x + y*y) - L3;
z = z - H;
d = sqrt(r*r+z*z);

float t = (L1*L1 + L2*L2 - d*d) / (2*L1*L2);
if (t >  1) t =  1;
if (t < -1) t = -1;
rad3 = PI - acos(t);
rad1 = atan2(y,x);
rad2 = atan2(z,r)+atan2(L2*sin(rad3),L1+L2*cos(rad3));

angle1 = rad1*(180/PI);
angle2 = rad2*(180/PI);
angle3 = rad3*(180/PI);

base_angle = 90 + angle1;
left_angle = angle2 - angle3 + 60;
right_angle = 150 - angle2;
}

// ========= 机械臂运动学逆解函数 =========
void ForwardKinematics() {
  angle1 = base.read()-90;
  angle2 = 150 - right.read();
  angle3 = angle2 - left.read()+60;

  rad1 = angle1 * (PI / 180);
  rad2 = angle2 * (PI / 180);
  rad3 = angle3 * (PI / 180);

  r = L1*cos(rad2) + L2*cos(rad2 - rad3);   
  z = L1*sin(rad2) + L2*sin(rad2 - rad3);

  r = r + L3;
  z = z + H;   
  x = r*cos(rad1);
  y = r*sin(rad1);
}

// ========= 直线预备函数 =========
void straight_line_prepare(float x1,float x2,float y1,float y2,float z1,float z2){
  lenth = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
  N = (int)ceil(lenth / step_size);
  if (N < 1) N = 1;
  dx = (x2 - x1) / N;
  dy = (y2 - y1) / N;
  dz = (z2 - z1) / N;
  x_now = x1;
  y_now = y1;
  z_now = z1;
  n = 0;
  next = true;
}

// ========= 直线函数 =========
 void straight_line(){
  if (next == true) {

  x_now = x_now + dx;
  y_now = y_now + dy;
  z_now = z_now + dz;
  n++;

  Kinematics(x_now, y_now, z_now);
  writeServo(base_angle, left_angle, right_angle);

  if (n >= N){
    next = false;
    n = 0;
  }
  }
}

// ========= 四舵机低通滤波运动函数 =========
void baseMove(int b_fromRos,int b_toRos){
  b_fromRos+=(b_toRos - b_fromRos)*0.25;
    base.write(b_fromRos);
    delay(runSpeed);
}

void leftMove(int l_fromRos,int l_toRos){
  l_fromRos+=(l_toRos - l_fromRos)*0.25;
    left.write(l_fromRos);
    delay(runSpeed);
}

void rightMove(int r_fromRos,int r_toRos){
  r_fromRos+=(r_toRos - r_fromRos)*0.25;
    right.write(r_fromRos);
    delay(runSpeed);
}

void clawMove(int c_fromRos,int c_toRos){
  c_fromRos+=(c_toRos-c_fromRos)*0.25;
    claw.write(c_fromRos);
    delay(runSpeed);
}

// =========任务三 循环 记录 播放 回中 函数=========
void task3(){
  if(record == true){
    if(recordStart == false){    
      i = 0;
    } else {                    
      temp = i - 1;                 
    }
    base_current_angle = base.read();
    left_current_angle = left.read();
    right_current_angle = right.read();
    claw_current_angle = claw.read();
    record = !record;
    recordStart = !recordStart;
    delay(300);
  }

  if(recordStart == true){
    joystick_control();
    if(i < 300){
      record1[i] = base.read();
      record2[i] = left.read();
      record3[i] = right.read();
      record4[i] = claw.read();
      i++;
    } else {
      recordStart = false;
      temp = 299;
    }
    delay(50);
  }

  if(play == true){
    base.write(base_current_angle);
    left.write(left_current_angle);
    right.write(right_current_angle);
    claw.write(claw_current_angle);
    delay(300);
    for(int k = 0; k <= temp; k++){
      base.write(record1[k]);
      left.write(record2[k]);
      right.write(record3[k]);
      claw.write(record4[k]);
      delay(50);
    }
    play = false;
  }

  if(back == true){
    baseMove(base.read(), 90);
    leftMove(left.read(), 90);
    rightMove(right.read(), 90);
    clawMove(claw.read(), 70);
    if(abs(base.read()-90)<=3&&abs(left.read()-90)<=3&&abs(right.read()-90)<=3&&abs(claw.read()-70)<=3){
      back = false;
      if(preDraw == true){    
      preDraw = false;
      end = true;
      showIndex = 0;
    }
    }
  }
}
// =========任务五 暂停 继续 取消 记录示教点 函数=========
void pause(){
  if(cmd=='p'){
    isPaused = true;
    cmd = 0;
}
}

void cancel(){
  if(cmd=='q'){
    cmd = 0;
    tenet = false;
    record = false; 
    play = false;
    taskA = false;
    taskAA = false;
    taskAAA = false;
    backA = false;
    taskB = false;
    taskBB = false;
    taskBBB = false;
    backB = false;
    taskC = false;
    taskCC = false;
    taskCCC = false;
    backC = false;
    preDraw = false;
    end = false;
    recordStart = false;
    isPaused = false;
    next = false;
    back = true;
  }
}

void start(){
  if(cmd=='s'){
    isPaused = false;
    cmd = 0;
}
}

void pointRecord(){
  if(cmd=='f'){
    angle1=base.read();
    angle2=left.read();
    angle3=right.read();
    ForwardKinematics();
    flag[0][pointIndex] = x;
    flag[1][pointIndex] = y;
    flag[2][pointIndex] = z;
    Serial.print("Point ");
    Serial.print(pointIndex+1);
    Serial.print(": (");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.print(z);
    Serial.println(")");
    pointIndex++;
    cmd=0;
    if(pointIndex>4){
      pointIndex=0;
    }
  }
}

void show(){
  if(end == true){
    if(next == false){         
      if(showIndex < 4){                
        x1 = flag[0][showIndex];
        y1 = flag[1][showIndex];
        z1 = flag[2][showIndex];
        x2 = flag[0][showIndex+1];
        y2 = flag[1][showIndex+1];
        z2 = flag[2][showIndex+1];
        straight_line_prepare(x1,x2,y1,y2,z1,z2);
        showIndex++;
      } else {
        end = false;            
        showIndex = 0;
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
delay(500); 
  while(Serial.available()) {
    Serial.read();   
  }
base.attach(9);
left.attach(8);
right.attach(7);
claw.attach(6);
base.write(90); 
left.write(90); 
right.write(90);
claw.write(70);
clawRos=70;
lastTick = millis();
delay(300); 
}


void loop() {
  if(Serial.available()){
    cmd = Serial.read();
  }
  start();
  cancel();
  pause();
  pointRecord();
if(isPaused){
  return;
}

if(cmd == 't'){
  tenet=true;
  cmd = 0;
}

if(tenet == true){
  if(circle >= 3) circle = 0;
  if(circle == 0){
    taskA = true;
  }
  else if(circle == 1){
    taskB = true;
  }
  else if(circle == 2){
    taskC = true;
  }
  circle++;
  tenet = false;   
}

if(cmd == 'P'){
  play=!play;
  cmd = 0;
}

if(cmd == 'R'){
  record=!record;
  cmd = 0;
}

if(cmd == 'o'){
  back=!back;
  cmd = 0;
}

if(cmd == 'A'){
  taskA=!taskA;
  cmd = 0;
}

if(cmd == 'B'){
  taskB=!taskB;
  cmd = 0;
}

if(cmd == 'C'){
  taskC=!taskC;
  cmd = 0;
}

if (cmd == 'z'){
    x1 = Serial.parseFloat();
    y1 = Serial.parseFloat();
    z1 = Serial.parseFloat();
    x2 = Serial.parseFloat();
    y2 = Serial.parseFloat();
    z2 = Serial.parseFloat();
    Kinematics(x1, y1, z1);
    writeServo(base_angle, left_angle, right_angle);
    delay(1000);
    straight_line_prepare(x1, x2, y1, y2, z1, z2);
    cmd = 0;                
}

if(cmd == 'e'){
  if(end == false){
    preDraw = true;
    back = true;     
  } else {
    end = false;      
  }
  cmd = 0;
}

if(cmd == 'm'){
  int xa = Serial.parseInt();   
  int ya = Serial.parseInt();
  int za = Serial.parseInt();
  base.write(xa);
  left.write(ya);
  right.write(za);
  cmd = 0;
}

unsigned long now_ms = millis();
  if (now_ms - lastTick >= TICK_MS){
    lastTick = now_ms;
    if (!isPaused){
      straight_line();
    }
}

task_1_basicControl();

joystick_control();

Amove();
Bmove();
Cmove();

task3();

show();
}