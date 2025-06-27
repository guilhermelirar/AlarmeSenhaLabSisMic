#include <msp430.h>
#include "lcd.h"
#include "led.h"
#include "utils.h"

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT
  P1DIR |= BIT0;                            // P1.0 set as output

  lcdPreInit();
  lcdWake();
  lcdWrite("Hello World!");

  timerConfig();
  __enable_interrupt();

  uint32_t ms = milis(); 
  
  while(1)                                  // continuous loop
  {
    led_Init();
    led_R_stt_Blink();
  }
}
