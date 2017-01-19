// Timer interrupt program: Generates an interrupt every 1 minute (or 1 second) which toggles the output on Pin 12 for the Iduino DUE Pro
// This will be used later to trigger the sensors in the cubesat to take a reading every minute for the telemetry specifications and can
// be modified for other timing requirements
// Note: there are 3 timers (TC0, TC1, TC2) each with 3 channels -> 9 possible timer compare interrupts

// Compare value to be compared to timer counter (32 bit): RC Compare value = (Desired period / (Prescaler / Master clock frequency))
// compareVal is at max 32 bits in length: 2^32 - 1 = 4294967295. This is equivalent to 6544s = 109 minutes with a 1:128 prescaler. 
// For period = 1s: compareVal = 1/(128/84E6) = 656250
// For period = 60s: compareVal = 60/(128/84E6) = 39375000
long compareVal = 39375000;           
char pin12State = 0;                // Character holding the current output of Pin 12

void timerInterruptSetup(void);     // Prototype for function to set up timer counter interrupts

void setup() {
  timerInterruptSetup();            // Call function to set up timer counter interrupts
  //pmc_enable_sleepmode();          // **TEST**Call function to activate sleep mode and executes __WFI() which waits for an interrupt to occur. When interrupt occurs, sleep mode is deactivated.


}

void loop() {
  
}


// Interrupt service routine for TC3 (corresponds to TC1 Ch0)
void TC3_Handler()
{
  TC_GetStatus(TC1, 0);            // Check status of TC1 Ch0 flag
  digitalWrite(12, pin12State = !pin12State);   // Toggle the output on Pin 12 ** REMOVE AFTER TESTING**
}

// Function to set up the timer counter interrupts
void timerInterruptSetup(void)
{
  //******************************REMOVE AFTER TESTING *********************************//
  pinMode(12, OUTPUT);              // Setup Pin 12                                    *//
  digitalWrite(12, LOW);            // Set Pin 12 low initially                        *//
  //************************************************************************************//
  pmc_set_writeprotect(false);      // Disable write protection on the Timer registers
  pmc_enable_periph_clk(TC3_IRQn);  // Enable built in clock and attach to TC3 handler (for TC1 Ch0)
  // Set up Timer Counter module for TC1 on Ch0 to be enabled. Timer clock 4 is used (MCLK/128 Prescaling). Waveform mode enabled in UP mode with auto trigger (clock reset) on RC compare match
  TC_Configure(TC1, 0, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4); 
  TC_SetRC(TC1, 0, compareVal);         // Set up RC value to be compared to TC1 Ch0 as compareVal
  REG_TC1_IER0=0b00010000;              // Enable RC compare interrupts on TC1
  REG_TC1_IDR0=0b11101111;              // Disable all other interrupts (timer counter overflow, RA, RB, etc)
  
  NVIC_ClearPendingIRQ(TC3_IRQn);       // Ensure TC3 interrupts (corresponds to TC1 Ch0) are not pending
  NVIC_EnableIRQ(TC3_IRQn);             // Enable TC3 interrupts
  __enable_irq();                       // Enable interrupts
  TC_Start(TC1, 0);                     // Start the timer counter on TC1 Ch0
}
