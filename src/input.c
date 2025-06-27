#include "input.h"
#include "utils.h"

void buttonsInit(void) 
{
  input.size = 0;
  input.last_input_time = 0;
  P1DIR &= ~BIT1;       // entrada
  P1REN |= BIT1;        // habilita resistor interno
  P1OUT |= BIT1;        // pull-up
  P1IES |= BIT1;        // interrupção por borda de descida
  P1IFG &= ~BIT1;       // limpa flag
  P1IE  |= BIT1;        // habilita interrupção

  // configuração análoga para s2 
  P2DIR &= ~BIT1;
  P2REN |= BIT1;
  P2OUT |= BIT1;
  P2IES |= BIT1;
  P2IFG &= ~BIT1;
  P2IE  |= BIT1;
}

Input* getInput() {
  if (milis() - input.last_input_time > 3000) {
    input.size = 0;
  }
  return &input;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
  if (P1IFG & BIT1) {

    _delay_cycles(22000);
    input.buffer[input.size++] = 1;
    input.last_input_time = milis();

    P1IFG &= ~BIT1;
  }
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
  if (P2IFG & BIT1) {

    _delay_cycles(22000);
    input.buffer[input.size++] = 2;
    input.last_input_time = milis();

    P2IFG &= ~BIT1;
  }
}


