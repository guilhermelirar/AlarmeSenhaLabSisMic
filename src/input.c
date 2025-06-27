#include "input.h"

void inputConfig(void) 
{
    
}

// ISR de P2
#pragma vector = PORT2_VECTOR
__interrupt void Port_2_ISR(void) {
  // Se S1 causou a interrupção
  if (P2IFG & S1) {

    debounce();

    if (~P2IN & S1) {
      P1OUT ^= LED1; // Alternar estado do LED
    }

    P2IFG &= ~BIT1; // Limpar flag
  }
}
