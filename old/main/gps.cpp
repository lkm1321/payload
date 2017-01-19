// Import the TinyGPS library
#include <TinyGPS.h>
#include "gps.h"
TinyGPS gps;

// Set up variables
int GPS_lock = 0;
int GPS_lock_new = 0;
int GPS_chars, GPS_sentences, GPS_failed_checksum;
// Set up variables to read
long GPS_lat, GPS_lon;
unsigned long GPS_fix_age, GPS_date, GPS_time, GPS_speed, GPS_course, chars;
unsigned short sentences, failed_checksum;

void GPSSetup() {
  Serial1.begin(9600);
  int GPS_TP_PIN = 22;
  int GPS_EN_PIN = 23;
  pinMode(GPS_TP_PIN, INPUT);
  pinMode(GPS_EN_PIN, OUTPUT);
  digitalWrite(GPS_EN_PIN, HIGH);
}

void GPSAcquire() {
  // Check for characters to read
  while (Serial1.available() > 0) {
    //Serial.println("Reading GPS data...");
    int c = Serial1.read();
    if (gps.encode(c)) {
      Serial.println("GPS data acquired");
      break;
    }
    //SerialUSB.print(char(c));
  }
  gps.get_position(&GPS_lat, &GPS_lon, &GPS_fix_age);
  gps.get_datetime(&GPS_date, &GPS_time, &GPS_fix_age);
  GPS_speed = gps.speed();
  GPS_course = gps.course();
}
