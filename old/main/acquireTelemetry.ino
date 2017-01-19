void acquireTelemetry() {
  // Read data from 3.3V current sensor
  // Read data from 5V current sensor
  // Read data from battery current 
  currentData = currentAcquire();
  // Read data from RFD temperature sensor
  // Read data from battery temperature sensor
  // Read data from external temperature sensor
  temperatureData = temperatureAcquire();
  // Get the time (GPS_time)
  GPSAcquire();
  // Save to disk
  // Check for GPS time acquisition
  if (GPS_time == -1 && 0) {
    // GPS time has not been acquired. Do not save WOD data.
    Serial.println("Time is not available. WOD data will not be saved...");
  } else {
    String filename = String(GPS_time % 10000000);
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
      Serial.println("Could not open file for WOD data");
    } else {
      file.println("Battery bus current (mA): " + String(currentData[1]));
      file.println("3.3V bus current (mA):    " + String(currentData[3]));
      file.println("5V bus current (mA):      " + String(currentData[5]));
      file.println("RFD temperature (C):      " + String(temperatureData[0]));
      file.println("Battery temperature (C):  " + String(temperatureData[1]));
      file.close();
    }
    Serial.println("WOD data at time " + String(GPS_time));
  }
  // Send the data over RFD/Debug
  Serial.println("Battery bus current (mA): " + String(currentData[1]));
  Serial.println("3.3V bus current (mA):    " + String(currentData[3]));
  Serial.println("5V bus current (mA):      " + String(currentData[5]));
  Serial.println("RFD temperature (C):      " + String(temperatureData[0]));
  Serial.println("Battery temperature (C):  " + String(temperatureData[1]));
}


