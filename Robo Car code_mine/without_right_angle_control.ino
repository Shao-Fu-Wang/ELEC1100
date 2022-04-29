// ELEC1100 WANG, Shao-fu
#define pinL_Sensor (A5)      //pin A5: left sensor 
#define pinB_Sensor (A4)      //pin A4: bumper sensor
#define pinR_Sensor (A3)      //pin A3: right sensor 
#define pinL_PWM (9)          //pin D9: left motor speed
#define pinL_DIR (10)         //pin D10: left motor direction
#define pinR_PWM (11)         //pin D11: right motor speed
#define pinR_DIR (12)         //pin D12: right motor direction
#define forward (1)
#define backward (0)
#define pwm_limit(l, r, min, max)  \
          if(l > max) l = max;  \
          if(r > max) r = max;  \
          if(l < min)  l = min; \
          if(r < min) r = min

bool leftSensor = 1;
bool rightSensor = 1;
bool bumpSensor = 1;
int bumpcount = 0;
int splitCount = 0;
int wheelSpeed = 180; 
bool split = false;
bool prevsplit = false;
enum state{
  stoping,
  tracking,
};
state cur_state = stoping;

void setup (){
  pinMode(pinL_Sensor, INPUT);
  pinMode(pinR_Sensor, INPUT);
  pinMode(pinB_Sensor, INPUT);
  pinMode(pinL_DIR, OUTPUT);
  pinMode(pinR_DIR, OUTPUT);
  pinMode(pinL_PWM, OUTPUT);
  pinMode(pinR_PWM, OUTPUT);
}

void gogogo(int lSpeed, int rSpeed, bool lDir, bool rDir){
  analogWrite(pinL_PWM, lSpeed);
  analogWrite(pinR_PWM, rSpeed);
  digitalWrite(pinL_DIR, lDir);
  digitalWrite(pinR_DIR, rDir); 
}

void loop(){
  leftSensor = digitalRead(pinL_Sensor);  rightSensor = digitalRead(pinR_Sensor); bumpSensor = digitalRead(pinB_Sensor);
  if(cur_state == stoping){ gogogo(0, 0, forward, forward); } //stoping
  if(!bumpSensor){  // starting and ending
    if(!bumpcount){
      cur_state = tracking;
      bumpcount++;
      gogogo(wheelSpeed, wheelSpeed, forward, forward);
      delay(300);
    }
    else if(bumpcount){
      cur_state = stoping;
      gogogo(wheelSpeed, wheelSpeed, backward, backward);
      delay(1100);
    }
  }
  else if(cur_state == tracking){
    if(!leftSensor && !rightSensor){  // split
        split = true;
        if(splitCount <= 2){  
          gogogo(wheelSpeed, wheelSpeed+66, backward, forward);
          delay(200); 
        }
        if(splitCount == 2){  
          gogogo(0, 0, forward, forward);
          delay(1000);  
        }
    }
    else{ // not split
      split = false;
      if(!leftSensor && rightSensor){ gogogo(wheelSpeed+66, wheelSpeed+66, backward, forward); } // go left
      if(leftSensor && !rightSensor){ gogogo(wheelSpeed+66, wheelSpeed+66, forward, backward); } // go right
      if(leftSensor && rightSensor) { gogogo(wheelSpeed, wheelSpeed, forward, forward); } // go forward
    }
    if (split == false && prevsplit == true){ splitCount++; }
    prevsplit = split;  // update splitting status
  }
}