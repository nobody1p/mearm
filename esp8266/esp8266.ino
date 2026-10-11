int button1,button2,button3,button4;
bool lastState1 = HIGH;
bool lastState2 = HIGH;
bool lastState3 = HIGH;
bool lastState4 = HIGH;
unsigned long pressTime=0;
bool wait = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(5,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(14,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  lastState1 = digitalRead(5);
  lastState2 = digitalRead(4);
  lastState3 = digitalRead(14);
  lastState4 = digitalRead(12);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool now1 = digitalRead(5);
  bool now2 = digitalRead(4);
  bool now3 = digitalRead(14);
  bool now4 = digitalRead(12);
  unsigned long t = millis();

if(lastState1 == HIGH && now1 == LOW){
  pressTime = t;
  wait = false;
}
if(lastState1 == LOW && now1 == LOW && !wait){
    if(t-pressTime>=500){
      Serial.write('pause');
      Serial.write('\n');
      wait = true;
    }
  }
  if(lastState1 == LOW && now1 == HIGH){
    if(!wait){
    Serial.write('tenet');
    Serial.write('\n');
  }
  wait = false;
  }
  lastState1 = now1;

if(lastState2 == HIGH && now2 == LOW){
  pressTime = t;
  wait = false;
}
  if(lastState2 == LOW && now2 == LOW && !wait){
    if(t-pressTime>=500){
      Serial.write('start');
      Serial.write('\n');
      wait = true;
    }
  }
  if(lastState2 == LOW && now2 == HIGH){
    if(!wait){
    Serial.write('record');
    Serial.write('\n');
  }
  wait = false;
  }
  lastState2 = now2;

if(lastState3 == HIGH && now3 == LOW){
  pressTime = t;
  wait = false;
}
  if(lastState3 == LOW && now3 == LOW && !wait){
    if(t-pressTime>=500){
      Serial.write('cancel');
      Serial.write('\n');
      wait = true;
    }
  }
  if(lastState3 == LOW && now3 == HIGH){
    if(!wait){
    Serial.write('play');
    Serial.write('\n');
  }
  wait = false;
  }
  lastState3 = now3;

  if(lastState4 == HIGH && now4 == LOW){
  pressTime = t;
  wait = false;
  }
  if(lastState4 == LOW && now4 == LOW && !wait){
    if(t-pressTime>=500){
      Serial.write('flag');
      Serial.write('\n');
      wait = true;
    }
  }
  if(lastState4 == LOW && now4 == HIGH){
    if(!wait){
    Serial.write('back');
    Serial.write('\n');
  }
  wait = false;
  }
  lastState4 = now4;
  }