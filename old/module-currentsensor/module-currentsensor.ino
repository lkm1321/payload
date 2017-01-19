#include <Wire.h>
#include <Adafruit_INA219.h>

float* currentsensor_acquire(void);     // Prototype for function which returns results from INA219 current sensor
void currentsensor_setup(void);         // Prototype for function which sets up 
Adafruit_INA219 ina219_battery;         // Setup INA219 instances - default 0x40 offset address for battery bus
Adafruit_INA219 ina219_3v3bus(0x41);    // A0 bridged for 3.3V bus (0x41 offset)
Adafruit_INA219 ina219_5vbus(0x44);     // A1 bridged for 5V bus (0x44 offset)

float *returnedCurrentSensorResults;   // Pointer to returned array of current sensor results

void setup(void) 
{
  Serial.begin(9600);     // Set up serial ***REMOVE AFTER TESTING***
  currentsensor_setup();    // Call function to set up current sensors
}

void loop(void) 
{
  int i;    // Counter *** REMOVE AFTER TESTING ***
  returnedCurrentSensorResults = currentsensor_acquire();   // Call currentsensor_acquire function to get results from current sensors. Return result array via pointer.

  //**************************************REMOVE AFTER TESTING**********************************************/
  Serial.print("\n"); Serial.print("RETURNED RESULTS\n");                                                  //
  for ( i = 0; i < 6; i++ ) {                                                                              //
    Serial.print(returnedCurrentSensorResults[i]); Serial.print("\n");                                     //
  }                                                                                                        //
  Serial.print("\n");                                                                                      //
  //********************************************************************************************************/ 
}

void currentsensor_setup(void)         // Function to set up current sensors
{           
  ina219_battery.begin();                       // Initialise battery bus INA219
  ina219_battery.setCalibration_32V_2A();    // Calibrate battery bus INA219 for +/- 2A range
  ina219_3v3bus.begin();                        // Initialise 3.3V bus INA219
  ina219_3v3bus.setCalibration_32V_2A();     // Calibrate 3.3V bus INA219 for +/- 2A range
  ina219_5vbus.begin();                         // Initialise 5V bus INA219
  ina219_5vbus.setCalibration_32V_2A();      // Calibrate 3V bus INA219 for +/- 2A range
}


float* currentsensor_acquire(void)     // Function which returns results from current sensors
{
  float shuntvoltage_battery, busvoltage_battery, current_mA_battery, loadvoltage_battery  ;     // Declare variables for storing voltage and current results
  float shuntvoltage_3v3bus, busvoltage_3v3bus, current_mA_3v3bus, loadvoltage_3v3bus;
  float shuntvoltage_5vbus, busvoltage_5vbus, current_mA_5vbus, loadvoltage_5vbus;
  static float currentSensorResults[6];         // Declare array to store voltage and current results to be returned
  
  shuntvoltage_battery = ina219_battery.getShuntVoltage_mV();     // Get voltage drop over shunt
  busvoltage_battery = ina219_battery.getBusVoltage_V();          // Get voltage between Vin- and GND
  current_mA_battery = ina219_battery.getCurrent_mA();            // Get current pulled over shunt
  loadvoltage_battery = busvoltage_battery + (shuntvoltage_battery / 1000);     // Calculate supply voltage on the battery bus 

  //**************************************REMOVE AFTER TESTING**********************************************/
  Serial.print("BATTERY BUS\n");                                                                           //
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage_battery); Serial.println(" V");                 //
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage_battery); Serial.println(" mV");              //
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage_battery); Serial.println(" V");                // 
  Serial.print("Current:       "); Serial.print(current_mA_battery); Serial.println(" mA");                //  
  Serial.println("");                                                                                      //
  delay(2000);                                                                                             //
  //********************************************************************************************************/ 
  
  shuntvoltage_3v3bus = ina219_3v3bus.getShuntVoltage_mV();        // Get voltage drop over shunt
  busvoltage_3v3bus = ina219_3v3bus.getBusVoltage_V();             // Get voltage between 3.3V bus and GND
  current_mA_3v3bus = ina219_3v3bus.getCurrent_mA();               // Get current pulled over shunt
  loadvoltage_3v3bus = busvoltage_3v3bus + (shuntvoltage_3v3bus / 1000);        // Calculate overall voltage on the 3.3V bus

  //**************************************REMOVE AFTER TESTING**********************************************/
  Serial.print("3V3 BUS\n");                                                                               //
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage_3v3bus); Serial.println(" V");                  //
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage_3v3bus); Serial.println(" mV");               //
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage_3v3bus); Serial.println(" V");                 //
  Serial.print("Current:       "); Serial.print(current_mA_3v3bus); Serial.println(" mA");                 //
  Serial.println("");                                                                                      //
  delay(2000);                                                                                             //
  //********************************************************************************************************/ 
  
  shuntvoltage_5vbus = ina219_5vbus.getShuntVoltage_mV();        // Get voltage drop over shunt
  busvoltage_5vbus = ina219_5vbus.getBusVoltage_V();             // Get voltage drop between 5V bus and GND
  current_mA_5vbus = ina219_5vbus.getCurrent_mA();               // Get current pulled over shunt
  loadvoltage_5vbus = busvoltage_5vbus + (shuntvoltage_5vbus / 1000);           // Calculate overall voltage on the 5V bus

  //**************************************REMOVE AFTER TESTING**********************************************/
  Serial.print("5V BUS\n");                                                                                //
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage_5vbus); Serial.println(" V");                   //
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage_5vbus); Serial.println(" mV");                //
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage_5vbus); Serial.println(" V");                  //
  Serial.print("Current:       "); Serial.print(current_mA_5vbus); Serial.println(" mA");                  //
  Serial.println("");                                                                                      //
  delay(2000);                                                                                             //
  //********************************************************************************************************/ 
  
  currentSensorResults[0] = loadvoltage_battery;    // Load results into results array
  currentSensorResults[1] = current_mA_battery;
  currentSensorResults[2] = loadvoltage_3v3bus;
  currentSensorResults[3] = current_mA_3v3bus;
  currentSensorResults[4] = loadvoltage_5vbus;
  currentSensorResults[5] = current_mA_5vbus;
  return currentSensorResults;                      // Return results array
}



