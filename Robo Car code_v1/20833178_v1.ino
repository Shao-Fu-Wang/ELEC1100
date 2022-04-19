/*
  ELEC1100 Your Lab#06 & Project Template

  To program the car tracking the white line on a dark mat

  Your name: WANG, Shao-fu
  
*/

// assign meaningful names to those pins that will be used

const int pinL_Sensor = A5;      //pin A5: left sensor 
const int pinB_Sensor = A4;      //pin A4: bumper sensor
const int pinR_Sensor = A3;      //pin A3: right sensor 

const int pinL_PWM = 9;          //pin D9: left motor speed
const int pinL_DIR = 10;         //pin D10: left motor direction

const int pinR_PWM = 11;         //pin D11: right motor speed
const int pinR_DIR = 12;         //pin D12: right motor direction

//define variables to be used in script

int bumperSensor = 1;  // not sensing white
int leftSensor = 1;    // not sensing white
int rightSensor = 1;   // not sensing white
int countSplit = 0;
int splitAdded = 0;
int stopped = 0;
int countBumper = 0;   // bumper not triggered yet
unsigned long long timestamp = 99999999;

// the setup function runs once when you press reset or power the board

void setup ()
{
  // define pins as input and output
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

// the loop function runs over and over again forever

void loop() {

  // Arduino is reading the sensor measurements
  bumperSensor = digitalRead(pinB_Sensor);
  leftSensor = digitalRead(pinL_Sensor);
  rightSensor = digitalRead(pinR_Sensor);
  
  // Car is reset at the start position
  if ( bumperSensor && countBumper == 0 ) {
    analogWrite(pinL_PWM, 0);
    analogWrite(pinR_PWM, 0);
  }

  // Bumper Sensor is triggered at the start position
  else if ( !bumperSensor && countBumper == 0) {
    analogWrite(pinL_PWM, 150);
    analogWrite(pinR_PWM, 150);
    countBumper = countBumper + 1;
    delay(350);     //to let the car leave the start position to avoid sensors miscount 00 at the start position 
  }
  
  // Car is tracking on the white line
  else if ( bumperSensor && countBumper == 1) 
  { 
    if((millis()-666) >= timestamp){
      if(stopped == 0){
        analogWrite(pinL_PWM, 0);
        analogWrite(pinR_PWM, 0);
        digitalWrite(pinL_DIR, 1);
        digitalWrite(pinR_DIR, 1);
        stopped = 1;
        delay(1000);
      }
    }
    if ( !leftSensor && !rightSensor ) {
        splitAdded = 0;
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, 0);
        digitalWrite(pinR_DIR, 1);
      }
    
    if ( !leftSensor && rightSensor ) {
        splitAdded = 0;
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, 0);
        digitalWrite(pinR_DIR, 1);  
      }
    
    if ( leftSensor && !rightSensor ) {
      splitAdded = 0;
      analogWrite(pinL_PWM, 150);
      analogWrite(pinR_PWM, 150);
      digitalWrite(pinL_DIR, 1);
      digitalWrite(pinR_DIR, 0);  
    }
    
    if ( leftSensor && rightSensor ) {
      if(splitAdded == 0){
        countSplit++;
        splitAdded = 1;
      }
      analogWrite(pinL_PWM, 150);
      analogWrite(pinR_PWM, 150);
      digitalWrite(pinL_DIR, 1);
      digitalWrite(pinR_DIR, 1); 
      
      if(countSplit == 3){
        timestamp = millis();
      }
      }
  }
  
  // Car is hitting the white wall
  else if ( !bumperSensor && countBumper == 1)
  {
    countBumper = countBumper + 1;
    analogWrite(pinL_PWM, 150);
    analogWrite(pinR_PWM, 150);
    digitalWrite(pinL_DIR, 0);
    digitalWrite(pinR_DIR, 0);  
    delay(1000);     // to let the car keep moving backward for 1 second
  }
  
  // Car stop after backward for 1 second 
  else if ( bumperSensor && countBumper == 2)
  {
    analogWrite(pinL_PWM, 0);
    analogWrite(pinR_PWM, 0);
  }
}