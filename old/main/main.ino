/*
CSP Satellite Code
2016
*/

/*
 * High-level overview of code:
 * Main loop
 *  - Listens for serial input from USB for debugging
 *  - Listens for data received over RFD
 *  - Process data in the background
 */

/* Include libraries */
#include <TimedAction.h>
#include <SD.h>
#include "temperature.h"
#include "current.h"
#include "gps.h"
#include "ADCS.h"
#include "payloadData.h"

/* Define constants for satellite modes */
#define MODE_INITIATION 0
#define MODE_SAFE 1
#define MODE_RECOVERY 2
#define MODE_STANDBY 3
#define MODE_PAYLOAD 4
#define MODE_TRANSMISSION 5
int mode;

/* Debugging to test timers */
int internal = 0;
void toggleInternal(){
  if (internal == 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    internal = 1;
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    internal = 0;
  }
}

int external = 0;
int externalPin = 22;
void toggleExternal() {
  if (external == 0) {
    digitalWrite(externalPin, HIGH);
    external = 1;
  } else {
    digitalWrite(externalPin, LOW);
    external = 0;
  }
}

// Holds flag to initiate command processing
int serialCommandAvailable = false;
// Stores a single input character
char input;
// Stores the total command
String command = "";
// Stores the motor drive speeds
char drivex = 0, drivey = 0, drivez = 0;
void serialInterface() {
  // Check if there is a command to execute
  if (serialCommandAvailable) {
    Serial.println(command);
    // Execute the command
    if (command == "START-TRANSMITTER") {
    } else if (command == "STOP-TRANSMITTER") {
    } else if (command == "DELETE-DATA") {
      // Delete data
      // Read in timestamp
      Serial.println("All satellite functions are halted - specify timestamp");
      bool lineReceived = false;
      String timestampString = "";
      char charTimestamp;
      // Read data until a new line character
      while (!lineReceived) {
        if (Serial.available()) {
          charTimestamp = Serial.read();
          if (charTimestamp == '\n') {
            // We have received a command
            lineReceived = true;
          } else {
            // Append the new character
            timestampString = timestampString + charTimestamp;
          }
        }
      }
      // Check the timestamp
      Serial.println("Timestamp received: " + timestampString);
      int timestampInteger = timestampString.toInt();
      if (timestampInteger == 0) {
        Serial.println("Timestamp is not an integer.");
      } else {
        Serial.println("Timestamp is valid, deleting files...");
        // Loop through the files on the SD card
        File dir = SD.open("/");
        File entry;
        String fileTimestampString;
        int fileTimestampInt;
        String entryName;
        while (true) {
          entry = dir.openNextFile();
          if (!entry) {
            break;
          } else {
            // Get the file name
            entryName = entry.name();
            // Convert to int
            fileTimestampInt = entryName.toInt();
            if (fileTimestampInt) {
              fileTimestampInt = fileTimestampInt % 10000000;
              if (fileTimestampInt < timestampInteger % 10000000) {
                // Delete the file
                SD.remove(entryName);
                Serial.println("Deleted file " + entryName);
              }
            }
          }
          entry.close();
        }
      }
      // Check if it is valid
    } else if (command == "MODE-SAFE") {
      Serial.println("Entering safe mode...");
      mode = MODE_SAFE;
    } else if (command == "MODE-RECOVERY") {
      Serial.println("Entering recovery mode...");
      mode = MODE_RECOVERY;
    } else if (command == "MODE-STANDBY") {
      Serial.println("Entering standby mode...");
      mode = MODE_STANDBY;
    } else if (command == "MODE-PAYLOAD") {
      Serial.println("Entering payload mode...");
      mode = MODE_PAYLOAD;
    } else if (command == "MODE-TRANSMISSION") {
      Serial.println("Entering transmission mode...");
      mode = MODE_TRANSMISSION;
    } else if (command == "DRIVE-X-PLUS") {
      // AIN1
      digitalWrite(31, HIGH);
      // AIN2
      digitalWrite(30, LOW);
      drivex++;
      analogWrite(2, drivex);
    } else if (command == "DRIVE-X-MINUS") {
      digitalWrite(31, HIGH);
      digitalWrite(30, LOW);
      drivex--;
      analogWrite(2, drivex);
    } else if (command == "DRIVE-Y-PLUS") {
      // CIN 1
      digitalWrite(10, HIGH);
      // CIN 2
      digitalWrite(9, LOW);
      drivey++;
      analogWrite(2, drivey);
    } else if (command == "DRIVE-Y-MINUS") {
      // CIN 1
      digitalWrite(10, HIGH);
      // CIN 2
      digitalWrite(9, LOW);
      drivey--;
      analogWrite(2, drivey);
    } else if (command == "DRIVE-Z-PLUS") {
      // DIN 1
      digitalWrite(11, HIGH);
      // DIN 2
      digitalWrite(12, LOW);
      drivez++;
      analogWrite(13, drivez);
    } else if (command == "DRIVE-Z-MINUS") {
      // DIN 1
      digitalWrite(11, HIGH);
      // DIN 2
      digitalWrite(12, LOW);
      drivez--;
      analogWrite(13, drivez);
    } else {
      // Invalid command
      Serial.println("Invalid command");
    }
    // Reset the flag
    serialCommandAvailable = false;
    command = "";
  // If there isn't a command to execute, read commands
  } else {
    // Check if there is data to read
    while (Serial.available() > 0 && !serialCommandAvailable) {
      // Get a single character
      input = Serial.read();
      // Check for newline to mark the end of a command
      if (input == '\n') {
        serialCommandAvailable = true;
      // Store the command
      } else {
        command = command + input;
      }
    }
  }
}


//create a couple timers that will fire repeatedly every x ms
TimedAction internalThread            = TimedAction(1000, toggleInternal);
TimedAction externalThread            = TimedAction(2000, toggleExternal);
// Check serial buffer once every second
TimedAction serialThread              = TimedAction(1000, serialInterface);
// Threads for individual modes
TimedAction modeSafeThread            = TimedAction(1000, modeSafe);
TimedAction modePayloadThread         = TimedAction(1000, modePayload);
TimedAction modeRecoveryThread        = TimedAction(1000, modeRecovery);
TimedAction modeStandbyThread         = TimedAction(1000, modeStandby);
TimedAction modeTransmissionThread    = TimedAction(1000, modeTransmission);
// Thread to acquire telemetry
TimedAction acquireTelemetryThread    = TimedAction(5000, acquireTelemetry);
TimedAction ADCSThread                = TimedAction(100, ADCSControl);

void setup() {
  Serial.begin(57600);
  Serial.println("Booting the CSP...");
  SerialUSB.begin(57600);
  SerialUSB.print("hello");
  /* Place the satellite in initiation mode */
  mode = MODE_INITIATION;
  // Set up the current sensors
  currentSetup();
  // Set up the temperature sensors
  temperatureSetup();
  // Set up the GPS
  GPSSetup();
  // Set up the reaction wheels
  ADCSSetup();
  // Set up the SD card
  bool SDStatus = SD.begin(4);
  if (SDStatus == false) {
    Serial.println("SD card not detected.");
  } else {
    Serial.println("SD card detected.");
  }
  // Set up the CCD
  payloadDataSetup();
  // Set up the motors
  pinMode(32, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(32, HIGH);
      /*  // AIN1
      digitalWrite(31, HIGH);
      // AIN2
      digitalWrite(30, LOW);
      drivex++;
      drivex = 10;
      analogWrite(2, drivex);

        // AIN1
      digitalWrite(9, HIGH);
      // AIN2
      digitalWrite(10, LOW);
      drivex++;
      drivex = 10;
      analogWrite(8, drivex);

        // AIN1
      digitalWrite(11, HIGH);
      // AIN2
      digitalWrite(12, LOW);
      drivex++;
      drivex = 10;
      analogWrite(13, drivex);*/
  // Test driving the motors
  /* Place the satellite into standby mode */
  Serial.println("CSP booted. Entering standby mode...");
  mode = MODE_STANDBY;
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(externalPin, OUTPUT);
}


void loop() {
  /* Execute code depending on the satellite mode */
  switch(mode) {  
    case MODE_INITIATION:
      break;
    case MODE_SAFE:
      modeSafeThread.check();
      break;
    case MODE_RECOVERY:
      Serial.println("Recovery mode...");
      modeRecoveryThread.check();
      Serial.println("Recover mode...2");
      break;
    case MODE_STANDBY:
      modeStandbyThread.check();
      break;
    case MODE_PAYLOAD:
      modePayloadThread.check();
      break;
    case MODE_TRANSMISSION:
      modeTransmissionThread.check();
      break;
    default:
      // Invalid mode - coding error.
      while (1) {
        Serial.println("Mode error");
        delay(5000);
      }
      break;
  }
  // Check threads
  internalThread.check();
  externalThread.check();
  serialThread.check();
}

