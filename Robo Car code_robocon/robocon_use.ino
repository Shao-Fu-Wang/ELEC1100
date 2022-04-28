//  good morning china, now i hace icecream
//  ELEC1100 tang yan yeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
const int pinL_Sensor = A5;      //pin A5: left sensor 
const int pinB_Sensor = A4;      //pin A4: bumper sensor
const int pinR_Sensor = A3;     //pin A3: right sensor 
const int pinL_PWM = 9;         //pin D9: left motor speed
const int pinL_DIR = 10;         //pin D10: left motor direction
const int pinR_PWM = 11;         //pin D11: right motor speed
const int pinR_DIR = 12;         //pin D12: right motor direction

bool leftSensor;
bool rightSensor;
bool bumpSensor;
uint8_t bumpcount = 0;
uint8_t splitCount = 0;
uint8_t ohSoFuckingFast = 180;
uint32_t imheretoeatmemory = 0;
uint32_t makeit100lines = 0;

#define cap_speedlimit(quantityA, quantityB, minvalue, maxvalue)  if(quantityA > maxvalue) quantityA = maxvalue;  if(quantityB > maxvalue) quantityB = maxvalue; if(quantityA < minvalue)  quantityA = minvalue; if(quantityB < minvalue) quantityB = minvalue
#define track 6
#define stop 9
#define ROBOSTATE \
    X(state)  
//give up x-macro

int iamsplitting = 0;
int iwassplitting = 0;
int mystate = 9;

void setup (){
  pinMode(pinL_Sensor, INPUT);
  pinMode(pinR_Sensor, INPUT);
  pinMode(pinB_Sensor, INPUT);
  pinMode(pinL_DIR, OUTPUT);
  pinMode(pinR_DIR, OUTPUT);
  pinMode(pinL_PWM, OUTPUT);
  pinMode(pinR_PWM, OUTPUT);
}

void mcif_independent_steering(long l, bool lDir, long r, bool rDir){
  cap_speedlimit(l, r, 0, 255);
  analogWrite(pinL_PWM, l);
  digitalWrite(pinL_DIR, lDir);
  analogWrite(pinR_PWM, r);
  digitalWrite(pinR_DIR, rDir); 
}

void loop(){
  imheretoeatmemory++;
  leftSensor = !digitalRead(pinL_Sensor);  
  rightSensor = !digitalRead(pinR_Sensor); 
  bumpSensor = digitalRead(pinB_Sensor);
  if(mystate == stop){ mcif_independent_steering(0, 1, 0, 1); } //stoping
  if(!bumpSensor){  // starting and ending
    if(bumpcount == 0){
      bumpcount++;
      mystate = track;
      mcif_independent_steering(ohSoFuckingFast, 1, ohSoFuckingFast, 1);
      delay(300);
    }
    else if(bumpcount == 1){  // go back at the end
      mystate = stop;
      mcif_independent_steering(ohSoFuckingFast, 0, ohSoFuckingFast, 0);
      delay(1100);
    }
  }
  else if(mystate == track){
    if(leftSensor && rightSensor){  // split
        iamsplitting = true;
        if(splitCount <= 2){  
          mcif_independent_steering(ohSoFuckingFast, 0, ohSoFuckingFast+69, 1);
          delay(200); 
        }
        if(splitCount == 2){  
          mcif_independent_steering(0, 0, 0, 0);
          delay(999);   // steal 1ms feel so happy
        } 
    }
    else{ // not split
      iamsplitting = false;
      if(leftSensor && !rightSensor){ 
        mcif_independent_steering(ohSoFuckingFast+69, 0, ohSoFuckingFast+69, 1); 
      } // go left
      if(!leftSensor && rightSensor){ 
        mcif_independent_steering(ohSoFuckingFast+69, 1, ohSoFuckingFast+69, 0); 
      } // go right
      if(!leftSensor && !rightSensor) { 
        mcif_independent_steering(ohSoFuckingFast, 1, ohSoFuckingFast, 1); 
      } // go forward
    }
    if (iamsplitting == false && iwassplitting == true){ 
      splitCount++; 
    }
    iwassplitting = iamsplitting;
    imheretoeatmemory--;
    makeit100lines++;
    makeit100lines++;
  }
}