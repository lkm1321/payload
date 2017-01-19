void modeRecovery() {
  // All systems on except payload and transceiver downlink
  acquireTelemetryThread.check();
  //ADCSThread.check();
}


