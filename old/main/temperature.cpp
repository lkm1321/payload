#include <Arduino.h>

float* temperatureAcquire(void)   // Function which gets the temperature sensor A/D converted values and converts to temperature in degrees C
{
  static float tempSensorResults[2];         // Declare array to store temperature results to be returned
  
  int analogtempVal_RFD=analogRead(A6);       // Read analog input from temperature sensor on RFD module and battery
  int analogtempVal_Battery=analogRead(A7);   // ##########CHANGE TO A6 (RFD temp) and A7 (Battery temp) for IDUINO DUE##########

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

void temperatureSetup(void)   // Function to set up the analog inputs for the temperature sensors
{
  //int A0 = 54;
  //int A1 = 55;
  pinMode(A0, INPUT);     // Set analog in pins to input ##########CHANGE TO A6 (RFD temp) and A7 (Battery temp) for IDUINO DUE##########
  pinMode(A1, INPUT);
}

