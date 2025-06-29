#include <msp430.h>
#include "lcd.h"
#include "led.h"
#include "utils.h"

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

  lcdPreInit();
  lcdWake();
  lcdWrite("Hello World!");

  led_Init();
  timerConfig();
  
  __enable_interrupt();

  while(1)                                  // continuous loop
  {
    led_R_stt_Blink(500);
  }
}
