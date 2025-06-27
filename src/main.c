#include <msp430.h>
#include "lcd.h"
#include "led.h"
#include "utils.h"
#include "fsm.h"

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT
  P1DIR |= BIT0;                            // P1.0 set as output

  lcdPreInit();
  led_Init();
  lcdWake();
  lcdWrite("Hello World!");

  timerConfig();
  __enable_interrupt();

  uint32_t ms = milis(); 
  
  while(1)                                  // continuous loop
  {
    updateState(); 
  }
}
