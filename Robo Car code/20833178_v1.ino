/*
  ELEC1100 Your Lab#06 & Project Template

  To program the car tracking the white line on a dark mat

  Your name: WANG, Shao-fu
*/
#define FORWARD 1
#define BACKWARD 0

// init pin
const int pinL_Sensor = A5;      //pin A5: left sensor 
const int pinB_Sensor = A4;      //pin A4: bumper sensor
const int pinR_Sensor = A3;      //pin A3: right sensor 
const int pinL_PWM = 9;          //pin D9: left motor speed
const int pinL_DIR = 10;         //pin D10: left motor direction
const int pinR_PWM = 11;         //pin D11: right motor speed
const int pinR_DIR = 12;         //pin D12: right motor direction

//sense = 1, non_sense = 0
int bumperSensor = 0;  // 0 = not sensing white
int leftSensor = 0;    // 0 = not sensing white
int rightSensor = 0;   // 0 = not sensing white
int countBumper = 0;   // bumper count
int countSplit = 0;   // split count

//user define 
enum states{
  initial,
  starting,
  tracking,
  ending
};
states robostate;
bool time_inited = 0;
unsigned long auto_tick = 0;
unsigned long start_tick = 0;
unsigned long L_last_bang_time = 0;
unsigned long L_this_bang_time = 0;
unsigned long R_last_bang_time = 0;
unsigned long R_this_bang_time = 0;
int L_speed = 150;
int R_speed = 150;

void setup ()
{
  pinMode(pinB_Sensor, INPUT);
  pinMode(pinL_Sensor, INPUT);
  pinMode(pinR_Sensor, INPUT);
  pinMode(pinL_DIR, OUTPUT);
  pinMode(pinR_DIR, OUTPUT);
  pinMode(pinL_PWM, OUTPUT);
  pinMode(pinR_PWM, OUTPUT);
  // initialize output pins
  digitalWrite(pinL_DIR, HIGH);   //forward direction    
  digitalWrite(pinR_DIR, HIGH);   //forward direction
  analogWrite(pinL_PWM, 0);    //stop at the start position 
  analogWrite(pinR_PWM, 0);    //stop at the start position 
}

int wheelEngine(int L_speed, int R_speed, bool dir = 1){
  if(dir){ // forward
    digitalWrite(pinL_DIR, HIGH);
    digitalWrite(pinR_DIR, HIGH);
  }
  if(!dir){ // backward
    digitalWrite(pinL_DIR, LOW);
    digitalWrite(pinR_DIR, LOW);  
  }
  analogWrite(pinL_PWM, L_speed);
  analogWrite(pinR_PWM, R_speed);
}

void get_input(){
  auto_tick = millis();
  bumperSensor = !digitalRead(pinB_Sensor);
  leftSensor = !digitalRead(pinL_Sensor);
  rightSensor = !digitalRead(pinR_Sensor);
}

void update_bang(){
  if(leftSensor){
    L_last_bang_time = L_this_bang_time;
    L_this_bang_time = auto_tick;
  }
  if(rightSensor){
    R_last_bang_time = R_this_bang_time;
    R_this_bang_time = auto_tick;
  }
}

void process_state(){
  if(robostate == initial){  // stop at init position
    wheelEngine(0, 0);
  }
  if(robostate == starting){ // go 0.35 sec after detecting bumper
    wheelEngine(L_speed, R_speed, FORWARD);
    countBumper++;
    if(time_inited == 0){
      start_tick = millis(); // get the starting time
      time_inited = 1; 
    }
    if(auto_tick - time_inited <= 350){
      wheelEngine(L_speed, R_speed, FORWARD);
    }
  }
  if(robostate == ending){ // wall in front
    countBumper++;
    wheelEngine(L_speed, R_speed, BACKWARD);  
    delay(1000);
    wheelEngine(0, 0);
  }
  if(robostate == tracking){ // main tracking
    update_bang();
    if (!leftSensor && !rightSensor){ // nothing detected
        wheelEngine(L_speed, R_speed, FORWARD);
    }
    if (!leftSensor && rightSensor){  // turn right
        wheelEngine(L_speed + 21, R_speed - 21, FORWARD);
    }
    if (leftSensor && !rightSensor){ // turn left
        wheelEngine(L_speed - 21, R_speed + 21, FORWARD);
    }
    if (leftSensor && rightSensor){ // split
        countSplit++;
        wheelEngine(L_speed - 66, R_speed + 66, FORWARD);
        delay(150);
    }
  }
}

void loop() { //main
  get_input();
  if (!bumperSensor && !countBumper){  
    robostate = initial;
  }
  else if (bumperSensor && !countBumper){ 
    robostate = starting;
  }
  else if (bumperSensor && countBumper){
    robostate = ending;
  }
  else if(!bumperSensor && countBumper!=2){  
    robostate = tracking;
  }
  process_state();
}