// Import the TinyGPS library
#include "TinyGPS.h"
TinyGPS gps;

// Set up the pins
int GPS_TX_PIN = 19;
int GPS_RX_PIN = 18;
int GPS_TP_PIN = 22;
int GPS_EN_PIN = 23;

// Set up variables
int GPS_lock = 0;
int GPS_lock_new = 0;
int GPS_chars, GPS_sentences, GPS_failed_checksum;
// Set up variables to read
long GPS_lat, GPS_lon;
unsigned long GPS_fix_age, GPS_time, GPS_date, GPS_speed, GPS_course, chars;
unsigned short sentences, failed_checksum;

void setup() {
  Serial1.begin(9600);
  SerialUSB.begin(9600);
  pinMode(GPS_TP_PIN, INPUT);
  pinMode(GPS_EN_PIN, OUTPUT);
  digitalWrite(GPS_EN_PIN, HIGH);
}

void loop() {
  // Check for characters to read
  while (Serial1.available() > 0) {
    int c = Serial1.read();
    if (gps.encode(c)) {
      SerialUSB.println("fuar");
    }
    //SerialUSB.print(char(c));
  }
  gps.get_position(&GPS_lat, &GPS_lon, &GPS_fix_age);
  gps.get_datetime(&GPS_date, &GPS_time, &GPS_fix_age);
  GPS_speed = gps.speed();
  GPS_course = gps.course();
}
