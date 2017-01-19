void modePayload() {
  // Downlink on standby
  // Reaction wheels on 70% power
  acquireTelemetryThread.check();
  //ADCSControl();
  // Display status
  Serial.println("Payload mode, acquiring data...");
  readCCD();
  sendData();
  //pinMode(25, HIGH); // This is a guess
  delay(100);
  readCCD();
  sendData();
  //pinMode(25, LOW);
}


