#include "payloadData.h"
#define SH 35 // 
#define ICG 36 // integration clear gate
#define MCLK 3 // master clock


uint8_t buffer[800];
uint8_t avg = 0;
char cmdBuffer[16];
int cmdIndex;
int exposureTime = 20;
long compareVal = 89;           
char MCLKState = 0;                // Character holding the current output of MCLK

void payloadDataSetup() {
  // put your setup code here, to run once:
  uint8_t val;

  // Initialize the clocks.
  //DDRB |= (SH | ICG | MCLK);  // Set the clock lines to outputs
  pinMode(MCLK, OUTPUT);
  pinMode(SH, OUTPUT);
  pinMode(ICG, OUTPUT);
  //CLOCK |= ICG;       // Set the integration clear gate high.
  digitalWrite(ICG, HIGH);

  // Enable the serial port.
  //Serial.begin(115200);

//  Setup timer2 to generate a 470kHz frequency on D11
//  TCCR2A =  + (0 << COM2A1) | (1 << COM2A0) | (1 << WGM21) | (0 << WGM20);
//  TCCR2B = (0 << WGM22) | (1 << CS20);
//  OCR2A = 20;
//  TCNT2 = 1;



// Set up the timer counter interrupts
  digitalWrite(MCLK, LOW);            // Set MCLK pin low initially                    *//
  //************************************************************************************//
  pmc_set_writeprotect(false);      // Disable write protection on the Timer registers
  pmc_enable_periph_clk(TC3_IRQn);  // Enable built in clock and attach to TC3 handler (for TC1 Ch0)
  // Set up Timer Counter module for TC1 on Ch0 to be enabled. Timer clock 4 is used (MCLK/2 Prescaling). Waveform mode enabled in UP mode with auto trigger (clock reset) on RC compare match
  TC_Configure(TC1, 0, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK1); 
  TC_SetRC(TC1, 0, compareVal);         // Set up RC value to be compared to TC1 Ch0 as compareVal
  REG_TC1_IER0=0b00010000;              // Enable RC compare interrupts on TC1
  REG_TC1_IDR0=0b11101111;              // Disable all other interrupts (timer counter overflow, RA, RB, etc)
  
  NVIC_ClearPendingIRQ(TC3_IRQn);       // Ensure TC3 interrupts (corresponds to TC1 Ch0) are not pending
  /*
  NVIC_EnableIRQ(TC3_IRQn);             // Enable TC3 interrupts
  __enable_irq();                       // Enable interrupts
  TC_Start(TC1, 0);                     // Start the timer counter on TC1 Ch0
  */

  // Set the ADC clock to around 0.65MHz
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX*2, ADC_STARTUP_NORM);
  
}

void readCCD(void)
{
  NVIC_EnableIRQ(TC3_IRQn);             // Enable TC3 interrupts
  __enable_irq();                       // Enable interrupts
  TC_Start(TC1, 0);                     // Start the timer counter on TC1 Ch0
  int x;
  uint8_t result;

  digitalWrite(ICG, LOW);
  delayMicroseconds(3);
  digitalWrite(SH, HIGH);
  delayMicroseconds(5);
  digitalWrite(SH, LOW);
  delayMicroseconds(15);
  digitalWrite(ICG, HIGH);
  delayMicroseconds(1);

  for (x = 0; x < 800; x++)
  {
    digitalWrite(SH, HIGH);
    if (x == 0)
    {
      avg = (uint8_t)(analogRead(A8) >> 2);
      result = (uint8_t)(analogRead(A8) >> 2);
    }
    else
    {
      result = (uint8_t)(analogRead(A8) >> 2);
      if (result < avg)
      {
        result = 0;
      }
      else
      {
        result -= avg;
      }
      buffer[x] = result;
      delayMicroseconds(20);
    }
    digitalWrite(SH, LOW);
  }
  __disable_irq();
}

void sendData(void)
{
  int x;

  for (x = 0; x < 800; ++x)
  {
    Serial.println(buffer[x]);
  }
}

// Interrupt service routine for TC3 (corresponds to TC1 Ch0)
void TC3_Handler()
{
  TC_GetStatus(TC1, 0);            // Check status of TC1 Ch0 flag
  digitalWrite(MCLK, MCLKState = !MCLKState);   // Toggle the output of MCLK pin
}

