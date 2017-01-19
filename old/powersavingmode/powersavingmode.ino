// NOTE: I tried to get Sleep mode (which is an inbuilt low power mode of the processor) to work but when I measured the current draw, it was the same as that under normal operation. People on 
// forums seem to report the same problem. Not sure if it's a problem with the coding or wait mode just does not work so well.
// This code uses an alternative way to save power found from http://www.udoo.org/forum/threads/reducing-sam3x-power-consumption-in-wait-or-sleep-mode.2176/ by turning off peripheral clocks.
// Test values: 78.70 mA current draw by the Iduino DUE Pro with all periph clocks off. 161.70 mA current draw with all periph clocks on.

void enablePowerSaving(void);   // Prototype for function which turns off peripheral clocks to save power
void disablePowerSaving(void);  // Prototype for function which turns on peripheral clocks again for normal operation

void setup() {
  pinMode(13, INPUT);              // Setup Pin 13
  pinMode(12, OUTPUT);              // Setup Pin 12
}

void loop() {
  if(digitalRead(13) == HIGH){
    enablePowerSaving();
  }
  else if(digitalRead(13) == LOW){
    disablePowerSaving();
  }
   
}

void enablePowerSaving(void)
{
  // To save power, disable peripheral clocks which are not used
  // From http://www.udoo.org/forum/threads/reducing-sam3x-power-consumption-in-wait-or-sleep-mode.2176/
  // NOTE: This will need to be editted such that the clocks which are necessary to 'wake up' the processor remain active
  pmc_disable_periph_clk(2);   // real-time clock
  pmc_disable_periph_clk(3);   // real-time timer
  pmc_disable_periph_clk(4);   // watchdog timer
  // 5 = PMC power mgmt ctrl

  pmc_disable_periph_clk(6);   // EEFC0  flash ctrl
  pmc_disable_periph_clk(7);   // EEFC1  flash ctrl
  // 8 = main UART

  pmc_disable_periph_clk(9);   // SMC_SDRAMC
  pmc_disable_periph_clk(10);  // SDRAMC

  //pmc_disable_periph_clk(11);  // PIO A     **** KEPT ON SO THAT THE I/O PORTS ARE STILL ACTIVE ****
  //pmc_disable_periph_clk(12);  // PIO B 
  //pmc_disable_periph_clk(13);  // PIO C 
  //pmc_disable_periph_clk(14);  // PIO D 
  //pmc_disable_periph_clk(15);  // PIO E 
  //pmc_disable_periph_clk(16);  // PIO F 

  pmc_disable_periph_clk(17);  // USART0  **** DEPENDING ON WHICH USART PORT USED FOR RF COMMS, LEAVE THAT PERIPH CLOCK RUNNING ****
  pmc_disable_periph_clk(18);  // USART1
  pmc_disable_periph_clk(19);  // USART2
  pmc_disable_periph_clk(20);  // USART3

  pmc_disable_periph_clk(21);  // HSMCI 
  pmc_disable_periph_clk(22);  // TWI/I2C bus 0
  pmc_disable_periph_clk(23);  // TWI/I2C bus 1
  pmc_disable_periph_clk(24);  // SPI0
  pmc_disable_periph_clk(25);  // SPI1
  pmc_disable_periph_clk(26);  // SSC

  pmc_disable_periph_clk(27);  // timer/counter 0
  pmc_disable_periph_clk(28);  // timer/counter 1
  pmc_disable_periph_clk(29);  // timer/counter 2
  pmc_disable_periph_clk(30);  // timer/counter 3
  pmc_disable_periph_clk(31);  // timer/counter 4
  pmc_disable_periph_clk(32);  // timer/counter 5
  pmc_disable_periph_clk(33);  // timer/counter 6
  pmc_disable_periph_clk(34);  // timer/counter 7
  pmc_disable_periph_clk(35);  // timer/counter 8
  pmc_disable_periph_clk(36);  // PWM
  pmc_disable_periph_clk(37);  // ADC

  pmc_disable_periph_clk(38);  // DAC ctrl
  pmc_disable_periph_clk(39);  // DMA ctrl
  pmc_disable_periph_clk(40);  // USB OTG high-speed ctrl
  pmc_disable_periph_clk(41);  // random number generator
  pmc_disable_periph_clk(42);  // ethernet MAC
  pmc_disable_periph_clk(43);  // CAN controller 0
  pmc_disable_periph_clk(44);  // CAN controller 1
}

void disablePowerSaving(void)
{
  // Re-enable all disabled clocks
  // NOTE: This will need to be editted such that only peripheral clocks which are necessary are enabled.
  // e.g. if the random number generator is never used, keep disabled.
  
  pmc_enable_periph_clk(2);   // real-time clock
  pmc_enable_periph_clk(3);   // real-time timer
  pmc_enable_periph_clk(4);   // watchdog timer
  // 5 = PMC power mgmt ctrl

  pmc_enable_periph_clk(6);   // EEFC0  flash ctrl
  pmc_enable_periph_clk(7);   // EEFC1  flash ctrl
  // 8 = main UART

  pmc_enable_periph_clk(9);   // SMC_SDRAMC
  pmc_enable_periph_clk(10);  // SDRAMC

  pmc_enable_periph_clk(11);  // PIO A 
  pmc_enable_periph_clk(12);  // PIO B 
  pmc_enable_periph_clk(13);  // PIO C 
  pmc_enable_periph_clk(14);  // PIO D 
  pmc_enable_periph_clk(15);  // PIO E 
  pmc_enable_periph_clk(16);  // PIO F 

  pmc_enable_periph_clk(17);  // USART0  
  pmc_enable_periph_clk(18);  // USART1
  pmc_enable_periph_clk(19);  // USART2
  pmc_enable_periph_clk(20);  // USART3

  pmc_enable_periph_clk(21);  // HSMCI 
  pmc_enable_periph_clk(22);  // TWI/I2C bus 0
  pmc_enable_periph_clk(23);  // TWI/I2C bus 1
  pmc_enable_periph_clk(24);  // SPI0
  pmc_enable_periph_clk(25);  // SPI1
  pmc_enable_periph_clk(26);  // SSC

  pmc_enable_periph_clk(27);  // timer/counter 0
  pmc_enable_periph_clk(28);  // timer/counter 1
  pmc_enable_periph_clk(29);  // timer/counter 2
  pmc_enable_periph_clk(30);  // timer/counter 3
  pmc_enable_periph_clk(31);  // timer/counter 4
  pmc_enable_periph_clk(32);  // timer/counter 5
  pmc_enable_periph_clk(33);  // timer/counter 6
  pmc_enable_periph_clk(34);  // timer/counter 7
  pmc_enable_periph_clk(35);  // timer/counter 8
  pmc_enable_periph_clk(36);  // PWM
  pmc_enable_periph_clk(37);  // ADC

  pmc_enable_periph_clk(38);  // DAC ctrl
  pmc_enable_periph_clk(39);  // DMA ctrl
  pmc_enable_periph_clk(40);  // USB OTG high-speed ctrl
  pmc_enable_periph_clk(41);  // random number generator
  pmc_enable_periph_clk(42);  // ethernet MAC
  pmc_enable_periph_clk(43);  // CAN controller 0
  pmc_enable_periph_clk(44);  // CAN controller 1
}

