// ELEC1100 WANG, Shao-fu
#define pinL_Sensor (A5)      //pin A5: left sensor 
#define pinB_Sensor (A4)      //pin A4: bumper sensor
#define pinR_Sensor (A3)      //pin A3: right sensor 
#define pinL_PWM (9)          //pin D9: left motor speed
#define pinL_DIR (10)         //pin D10: left motor direction
#define pinR_PWM (11)         //pin D11: right motor speed
#define pinR_DIR (12)         //pin D12: right motor direction
// self define
#define cap_speedlimit(quantityA, quantityB, minvalue, maxvalue)  if(quantityA > maxvalue) quantityA = maxvalue;  if(quantityB > maxvalue) quantityB = maxvalue; if(quantityA < minvalue)  quantityA = minvalue; if(quantityB < minvalue) quantityB = minvalue
#define forward (1)
#define backward (0)

bool leftSensor = 1;
bool rightSensor = 1;
bool bumpSensor = 1;
uint8_t bumpcount = 0;
uint8_t splitCount = 0;
int32_t wheelSpeed = 200; // using int32_t in case number over 255
bool split;
bool prevsplit;
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
  digitalWrite(pinL_DIR, HIGH);   //forward direction    
  digitalWrite(pinR_DIR, HIGH);   //forward direction
  analogWrite(pinL_PWM, 0);    //forward speed 
  analogWrite(pinR_PWM, 0);    //forward speed 
}

void gogogo(uint8_t lSpeed, uint8_t rSpeed, bool lDir = forward, bool rDir = forward){
  cap_speedlimit(lSpeed, rSpeed, 0, 255);
  analogWrite(pinL_PWM, lSpeed);
  analogWrite(pinR_PWM, rSpeed);
  digitalWrite(pinL_DIR, lDir);
  digitalWrite(pinR_DIR, rDir); 
}

void loop(){
  leftSensor = digitalRead(pinL_Sensor);  rightSensor = digitalRead(pinR_Sensor); bumpSensor = digitalRead(pinB_Sensor);
  if(cur_state == stoping){ gogogo(0, 0); } //stoping
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
  if(cur_state == tracking){
    if(!leftSensor && !rightSensor){  // split
        split = true;
        if(splitCount <= 3){  // in case miscount more splits, do nothing when splitCount is 4
          gogogo(wheelSpeed-50, wheelSpeed, backward, forward);
          delay(200); 
        }
        if(splitCount == 3){  delay(1000);  } // stop at the third stop
    }
    else{ // not split
      split = false;
      if(!leftSensor && rightSensor){ gogogo(wheelSpeed, wheelSpeed, backward, forward); } // go left
      if(leftSensor && !rightSensor){ gogogo(wheelSpeed, wheelSpeed, forward, backward); } // go right
      if(leftSensor && rightSensor) { gogogo(wheelSpeed, wheelSpeed, forward, forward); } // go forward
    }
    if (!split && prevsplit){ splitCount++; }
    prevsplit = split;  // update splitting status
  }
}