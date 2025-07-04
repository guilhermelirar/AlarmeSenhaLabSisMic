#include <msp430.h>
#include "lcd.h"
#include "led.h"
#include "utils.h"
#include "fsm.h"
#include "input.h"

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

  // Inicialização de módulos
  lcdPreInit();
  led_Init();
  inputInit();
  timerConfig();
        
  // Interrupções
  __enable_interrupt();

  while(1)                                  // continuous loop
  {
    inputUpdate();   
    updateState();
    led_update();
  }
}
