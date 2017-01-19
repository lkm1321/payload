#include <Wire.h>
#include <LPS.h>

LPS ps;

void setup()
{
  SerialUSB.begin(9600);
  SerialUSB.println("lmbo");
  Wire1.begin();

  if (!ps.init())
  {
    SerialUSB.println("Failed to autodetect pressure sensor!");
    while (1);
  }

  ps.enableDefault();
}

void loop()
{
  float pressure = ps.readPressureMillibars();
  float altitude = ps.pressureToAltitudeMeters(pressure);
  float temperature = ps.readTemperatureC();
  
  SerialUSB.print("p: ");
  SerialUSB.print(pressure);
  SerialUSB.print(" mbar\ta: ");
  SerialUSB.print(altitude);
  SerialUSB.print(" m\tt: ");
  SerialUSB.print(temperature);
  SerialUSB.println(" deg C");

  delay(100);
}
