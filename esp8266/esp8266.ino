#include <Wire.h>
int button1,button2,button3,button4;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(100000);
  pinMode(0,INPUT_PULLUP);
  pinMode(13,INPUT_PULLUP);
  pinMode(14,INPUT_PULLUP);
  pinMode(15,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
button1=digitalRead(0);
button2=digitalRead(14);
button3=digitalRead(12);
button4=digitalRead(13);
if(button1==0){
  delay(300);
  if(button2==0){
    Wire.beginTransmission(8);
    Wire.write("s");
    Wire.endTransmission();
    return;
    
  }
    Wire.beginTransmission(8);
    Wire.write("t");
    Wire.endTransmission();
    return;
}
if(button2==0){
  delay(300);
  if(button3==0){
    Wire.beginTransmission(8);
    Wire.write("o");
    Wire.endTransmission();
    return;
  }
  Wire.beginTransmission(8);
    Wire.write("r");
    Wire.endTransmission();
    return;
}
if(button3==0){
  delay(300);
  if(button4==0){
    Wire.beginTransmission(8);
    Wire.write("c");
    Wire.endTransmission();
    return;
  }
    Wire.beginTransmission(8);
    Wire.write("p");
    Wire.endTransmission();
    return;
}
if(button4==0){
  Wire.beginTransmission(8);
  Wire.write("b");
  Wire.endTransmission();
  Serial.println("a");
  return;
}
Serial.println("b");
}
