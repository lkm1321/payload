/*
Arduino Protothreading Example v1.0
by Drew Alden (@ReanimationXP) 1/12/2016
*/

//COMPONENTS

/*
This code was made using the Sunfounder Arduino starter kit's blue LCD.
It can be found at Amazon.com in a variety of kits.
*/

//THIRD-PARTY LIBRARIES

    //these must be manually added to your Arduino IDE installation
    //see sites for details.

    //gives us the ability to do a foreach loop:
    //http://playground.arduino.cc/Code/Utility
    //#include <Utility.h>
    
    //allows us to set actions to perform on separate timed intervals
    //http://playground.arduino.cc/Code/TimedAction
    //http://wiring.uniandes.edu.co/source/trunk/wiring/firmware/libraries/TimedAction
    #include <TimedAction.h>

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

//create a couple timers that will fire repeatedly every x ms
TimedAction internalThread = TimedAction(1000, toggleInternal);
TimedAction externalThread = TimedAction(2000, toggleExternal);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(externalPin, OUTPUT);
}


void loop() {
  
  //check on our threads. based on how long the system has been
  //running, do they need to fire and do work? if so, do it!
  internalThread.check();
  externalThread.check();
}
