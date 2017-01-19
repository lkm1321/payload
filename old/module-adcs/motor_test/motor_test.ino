//***********reactionwheelcontrol.c*****************************//
// This function takes in an analog input and passes the error 
// through a PID loop which drives the motor.
// The closer the analog input is to the desired value "SetPt", the slower
// the motor spins and vise vera.
// 

// Pin Outs
// Pin 2 = PWM
// Pin 30 = AIN2
// Pin 31 = AIN1
// Pin 32 = STBY 
// Pin A1 = Analog input, connect to potentiometer or something

#include <Wire.h>
#include <L3G.h>

L3G gyro;

//***************Tuning Variables**********************//
int IntThresh = 10000;  //prevents windup error.
float kI = 0;     //keep this low
float kD =0;       //0.1 ish is ok
float kP = 1;         // tune this later
int ScaleFactor = 4;  //10 bit to 8 bit conversion of drive. 
                      //Because Drive = P+I+D and P,I,D are 10 bit and drive is 8 bit
int SetPt = 1;      //desired analog input. Closer to this number the better
//******************************************************//

//Misc Variables to be defined
int Integral = 0;
int Last = 0;
char dutyCycle = 100;


void setup() {
  Serial.begin(9600);
  Wire1.begin();
  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }
  gyro.enableDefault();
  
//  pinMode(2, OUTPUT);       //PWM on PIN2
//  pinMode(30, OUTPUT);      //AIN2 direction pin 2
//  pinMode(31, OUTPUT);      //AIN1 direction pin 1
//  pinMode(32, OUTPUT);      //STBY, standby pin pulled high
//  digitalWrite(32, HIGH);   // Standby pin on motor driver (pull HIGH)

  pinMode(8, OUTPUT);       //PWM on PIN2
  pinMode(9, OUTPUT);      //AIN2 direction pin 2
  pinMode(10, OUTPUT);      //AIN1 direction pin 1
  pinMode(32, OUTPUT);      //STBY, standby pin pulled high
  digitalWrite(32, HIGH);   // Standby pin on motor driver (pull HIGH)
 
}

void loop() {
  gyro.read();              // Acquire angular velocity readings from IMU
  int Actual = (int)(gyro.g.z/114);  //Read z-axis angular velocity from IMU
  int Error = Actual;   //error calc in 10bit
  Serial.print("Actual = "); Serial.print(Actual);         //for debugging
  
  if (abs(Error) < IntThresh){  // prevent integral 'windup'
  Integral = Integral + Error;  // accumulate the error integral
  }
  else {
  Integral=0; // zero it if out of bounds
  }
  float P = Error*kP; // calc proportional term
  float I = Integral*kI; // integral term
  float D = (Last-Actual)*kD; // derivative term
  int Drive = P + I + D; // Total drive = P+I+D (10bit)
  Drive = Drive/ScaleFactor; // scale Drive to be in the range 0-255 (8bit)
  
  //********************Drive the motor***************************//
  // This section of code drives the motor and controls direction
  // that the motor spins.
  // Drive is a 8 bit value and will saturate if above.
  //**************************************************************//
  if (Drive < 0){ // Check which direction to go.
    digitalWrite(10, HIGH); //CW i think, need to be callibrated later
    digitalWrite(9, LOW);
    
  }
  else { // depending on the sign of Error
    digitalWrite(9, HIGH); //CCW i think
    digitalWrite(10, LOW);
  }
  
  if (abs(Drive)>255) { //Drive has saturated, so set to 255 (max duty/speed)
  Drive=255;
  }

  if (Drive == 0){
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
  }
  else{
    analogWrite(8, abs(Drive)); // send PWM command to motor board
  }
  
  
  //**************************************************************//
  
  Last = Actual; // save current value for next time 

  Serial.print("\tError = "); Serial.print(Error); Serial.print("\tP = "); Serial.print(P); Serial.print("\tI = "); Serial.print(I); Serial.print("\tD = "); 
  Serial.print(D); Serial.print("\tDrive = "); Serial.print(Drive); Serial.print("\n"); //print to serial monitor

}
