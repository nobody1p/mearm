#include <Servo.h>
char command;
int runSpeed;
int clawRos;
const int rosMin=0;
const int baseMax;
const int rightMax;
const int leftMax;
const int clawMax;
const int runSpeedMax;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Servo.base;
Servo.right;
Servo.left;
Servo.claw;
base.attach(S1);
left.attach(S2);
right.attach(S3);
claw.attach(S4);

}

void loop() {
  // put your main code here, to run repeatedly:
command=Serial.read();
switch(command){
case'o':
case'O':
  clawRos= claw.read();
  clawRos+=10;
  if(clawRos>clawMax){
    Serial.println("已经达到最大钳宽");
    clawRos-=10;
    break;
  }
claw.write(clawRos);
Serial.print("当前钳宽：”);
Serial.println(clawRos);
break;

case's':
case'S':
  clawRos= claw.read();
  clawRos-=10;
  if(clawRos<=0){
    Serial.println("已经达到最小钳宽");
    clawRos+=10;
    break;
  }
claw.write(clawRos);
Serial.print("当前钳宽：”);
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
  Serial.print("当前运行速度：”);
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
  Serial.print("当前运行速度：”);
  Serial.println(runSpeed);
  break;

  default:
    break;
}
/*以上为任务一中mearm机械臂的基础控制模块
关于多舵机的协调控制模块，我想完成一个算法，建立以claw为坐标原点，垂直地面为z轴，垂直
前面板为y轴，平行前面板为x轴的坐标系，使机械臂能根据base right left三个舵机的角度数据，
计算出claw的实时坐标位置，实现精准抓取，目前正在思考实现方法*/
}
