#include <Adafruit_MCP9808.h>

float* temperatureAcquire(void);     // Prototype for function which returns results from INA219 current sensor
void temperatureSetup(void);         // Prototype for function which sets up 

float *temperatureData;   // Pointer to returned array of current sensor results
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

