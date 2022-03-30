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
int countSplit = -1;   // split count

//user define 
enum states{
  initial,
  starting,
  tracking,
  ending
};
enum trackMode{
  straight,
  turnLeft,
  turnRight,
  split,
};
states robostate = initial;
trackMode robotrack = straight;
// bool time_inited = 0; // use for timestamp, remember to set it back to 0
// bool countSplit_added = 0;  // use for adding countSplit, remember to set it back to 0
unsigned long auto_tick = 0;
unsigned long timestamp = 0;
unsigned long R_bang_time = 0;
unsigned long L_bang_time = 0;
// need tuning
int turnmap[3] = {0, 0, 0};  // 0 for turnleft 
int init_L_speed = 150;
int init_R_speed = 150;
int L_speed = 150;
int R_speed = 150;
int differential = 21; // for changing directions with pid
int Kt = 3;

void setup(){
  Serial.begin(38400); // should be deleted
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
void get_input(){
  auto_tick = millis();
  bumperSensor = !digitalRead(pinB_Sensor);
  leftSensor = !digitalRead(pinL_Sensor);
  rightSensor = !digitalRead(pinR_Sensor);
}
void update_bang(){
  if(leftSensor){
    L_bang_time = auto_tick;
  }
  if(rightSensor){
    R_bang_time = auto_tick;
  }
}
void wheelEngine(int L_speed, int R_speed, bool L_dir = 1, bool R_dir = 1){
  if(L_dir){ // forward
    digitalWrite(pinL_DIR, HIGH);
  }
  if(R_dir){ // forward
    digitalWrite(pinL_DIR, HIGH);
  }
  if(!L_dir){ // backward
    digitalWrite(pinL_DIR, LOW); 
  }
  if(!R_dir){ // backward
    digitalWrite(pinR_DIR, LOW); 
  }
  if(L_speed > 255){
    L_speed = 255;
  }
  if(R_speed > 255){
    R_speed = 255;
  }
  if(L_speed < 0){
    L_speed = 0;
  }
  if(R_speed < 0){
    R_speed = 0;
  }
  analogWrite(pinL_PWM, L_speed);
  analogWrite(pinR_PWM, R_speed);
}

void determine_trackmode(){
  if(abs(R_bang_time - L_bang_time) < 333){ // determine split
    countSplit++;
    robotrack = split;
  }
  else if(!leftSensor && rightSensor){  // turn right
    robotrack = turnRight;
  }
  else if(leftSensor && !rightSensor){  // turn left
    robotrack = turnLeft;
  }
  else if(!leftSensor && rightSensor){  // go straight
    robotrack = straight;
  } 
}

void process_state(){
  if(robostate == initial){  // stop at init position
    wheelEngine(0, 0);
  }
  if(robostate == starting){ // go 0.35 sec after detecting bumper
    countBumper++;
    wheelEngine(L_speed, R_speed, FORWARD, FORWARD);
    delay(350);
  }
  if(robostate == ending){ // wall in front
    countBumper++;
    wheelEngine(L_speed, R_speed, BACKWARD, BACKWARD);  
    delay(1000);
    wheelEngine(0, 0);
  }
  if(robostate == tracking){ // main tracking
    update_bang();
    determine_trackmode();
    L_speed = init_L_speed;
    R_speed = init_R_speed;
    if(robotrack == turnLeft){
      R_speed += differential;
    }
    if(robotrack == turnRight){
      L_speed += differential;
    }
    if(robotrack == split){ // turnmap 0 == turnleft
      wheelEngine(L_speed, R_speed, turnmap[countSplit], 1-turnmap[countSplit]);
      delay(333);
    }
    wheelEngine(L_speed, R_speed, FORWARD, FORWARD);
  }
}

void loop() { //main
  get_input();
  Serial.println(auto_tick);
  Serial.println(robostate);
  if(!bumperSensor && !countBumper){  
    robostate = initial;
  }
  else if(bumperSensor && !countBumper){ 
    robostate = starting;
  }
  else if(bumperSensor && countBumper){
    robostate = ending;
  }
  else if(!bumperSensor && countBumper!=2){
    robostate = tracking;
  }
  process_state();
}