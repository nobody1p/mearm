int button1,button2,button3,button4;
bool lastState1 = HIGH;
bool lastState2 = HIGH;
bool lastState3 = HIGH;
bool lastState4 = HIGH;
unsigned long pressTime1=0;
unsigned long pressTime2=0;
unsigned long pressTime3=0;
unsigned long pressTime4=0;
bool wait1 = false;
bool wait2 = false;
bool wait3 = false;
bool wait4 = false;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(200);
  for(int i = 0; i < 150; i++) {
    Serial.write('\n');
  }
  delay(100);
  while(Serial.available()) {
    Serial.read();
  }
  pinMode(5,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(13,INPUT_PULLUP);
  pinMode(0,INPUT_PULLUP);
  lastState1 = digitalRead(5);
  lastState2 = digitalRead(4);
  lastState3 = digitalRead(13);
  lastState4 = digitalRead(0);
  Serial.println("ESP8266 ready");
}

void loop() {

  // put your main code here, to run repeatedly:
  bool now1 = digitalRead(5);
  bool now2 = digitalRead(4);
  bool now3 = digitalRead(13);
  bool now4 = digitalRead(0);
  unsigned long t = millis();
  
   if(Serial.available()) {
    Serial.write(Serial.read());
  }

if(lastState1 == HIGH && now1 == LOW){
  pressTime1 = t;
  wait1 = false;
}
if(lastState1 == LOW && now1 == LOW && !wait1){
    if(t-pressTime1>=500){
      Serial.write('p');
      Serial.println("\n");
      wait1 = true;
      delay(500);
    }
  }
if(lastState1 == LOW && now1 == HIGH){
    if(!wait1){
    Serial.write('t');
    Serial.println("\n");
    delay(500);
  }
  wait1 = false;
  }
  lastState1 = now1;

if(lastState2 == HIGH && now2 == LOW){
  pressTime2 = t;
  wait2 = false;
}
  if(lastState2 == LOW && now2 == LOW && !wait2){
    if(t-pressTime2>=500){
      Serial.write('s');
      Serial.println("\n");
      wait2 = true;
      delay(500);
    }
  }
  if(lastState2 == LOW && now2 == HIGH){
    if(!wait2){
    Serial.write('R');
    Serial.println("\n");
    delay(500);
  }
  wait2 = false;
  }
  lastState2 = now2;

if(lastState3 == HIGH && now3 == LOW){
  pressTime3 = t;
  wait3 = false;
}
  if(lastState3 == LOW && now3 == LOW && !wait3){
    if(t-pressTime3>=500){
      Serial.write('q');
      Serial.println("\n");
      wait3 = true;
      delay(500);
    }
  }
  if(lastState3 == LOW && now3 == HIGH){
    if(!wait3){
    Serial.write('P');
    Serial.println("\n");
    delay(500);
  }
  wait3 = false;
  }
  lastState3 = now3;

  if(lastState4 == HIGH && now4 == LOW){
  pressTime4 = t;
  wait4 = false;
  }
  if(lastState4 == LOW && now4 == LOW && !wait4){
    if(t-pressTime4>=500){
      Serial.write('f');
      Serial.println("\n");
      wait4 = true;
      delay(500);
    }
  }
  if(lastState4 == LOW && now4 == HIGH){
    if(!wait4){
    Serial.write('o');
    Serial.println("\n");
    delay(500);
  }
  wait4 = false;
  }
  lastState4 = now4;
  }
