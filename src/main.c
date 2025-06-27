#include <msp430.h>
#include "lcd.h"
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
    if (milis() - ms >= 1000) {             // 1 segundo se passou
      P1OUT ^= BIT0;                        // XOR P1.0
      ms = milis();                         // Atualiza milis
    }
  }
}
