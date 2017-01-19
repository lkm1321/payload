#include <Wire.h>
//#include "Adafruit_MCP9808.h"
#include <Adafruit_MCP9808.h>

//**************************************REMOVE AFTER TESTING**********************************************/
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();                                                        //     
//********************************************************************************************************/

float* tempsensor_acquire(void);     // Prototype for function which returns results from INA219 current sensor
void tempsensor_setup(void);         // Prototype for function which sets up 

float *returnedTempSensorResults;   // Pointer to returned array of current sensor results

void setup() {
  tempsensor_setup();       // Call function to set up temperature sensors
  //**************************************REMOVE AFTER TESTING**********************************************/
  Serial.begin(9600);                                                                                      // 
  tempsensor.begin();                                                                                      //
  //********************************************************************************************************/
}

void loop() {

  returnedTempSensorResults = tempsensor_acquire();    // Call tempsensor_acquire function to attain temperature readings from sensors on RFD module and battery. Return result array via pointer.

  //**************************************REMOVE AFTER TESTING**********************************************/
  Serial.print("\n"); Serial.print(returnedTempSensorResults[0]); Serial.print("\n");
  Serial.print(returnedTempSensorResults[1]); Serial.print("\n");
  //*************************************USED FOR MCP9808 FOR CALIBRATION***********************************/
  tempsensor.shutdown_wake(0);   // Don't remove this line! required before reading temp                   //
  float c = tempsensor.readTempC();                                                                        // 
  Serial.print("Temp: "); Serial.print(c); Serial.print("*C\t");                                           // 
  delay(250);                                                                                              //
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere                   //
  delay(2000);                                                                                             //
  //*********************************************************************************************************/
}

void tempsensor_setup(void)   // Function to set up the analog inputs for the temperature sensors
{
  pinMode(A0, INPUT);     // Set analog in pins to input ##########CHANGE TO A6 (RFD temp) and A7 (Battery temp) for IDUINO DUE##########
  pinMode(A1, INPUT);
}

float* tempsensor_acquire(void)   // Function which gets the temperature sensor A/D converted values and converts to temperature in degrees C
{
  static float tempSensorResults[2];         // Declare array to store temperature results to be returned
  
  int analogtempVal_RFD=analogRead(A0);       // Read analog input from temperature sensor on RFD module and battery
  int analogtempVal_Battery=analogRead(A1);   // ##########CHANGE TO A6 (RFD temp) and A7 (Battery temp) for IDUINO DUE##########

  // A/D value to temperature (in degrees C) value conversion algorithm from https://tkkrlab.nl/wiki/Arduino_KY-013_Temperature_sensor_module
  double interm_temp_RFD = log(10000.0*((1024.0/analogtempVal_RFD-1))); 
  interm_temp_RFD = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * interm_temp_RFD * interm_temp_RFD))* interm_temp_RFD);
  float temp_RFD = interm_temp_RFD - 272.35; 
  double interm_temp_Battery = log(10000.0*((1024.0/analogtempVal_Battery-1))); 
  interm_temp_Battery = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * interm_temp_Battery * interm_temp_Battery))* interm_temp_Battery);
  float temp_Battery = interm_temp_Battery - 272.35; 

  tempSensorResults[0] = temp_RFD;        // Store temperature values to an array
  tempSensorResults[1] = temp_Battery;
  return tempSensorResults;               // Return array
}
 
