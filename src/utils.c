#include "utils.h"
#include <msp430.h>

static volatile uint32_t ms = 0;

uint32_t milis(void) {
  return ms;
}

void uitoascii(uint8_t value, char *buffer) {
  if (value == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
    return;
  }

  char temp[3];   
  uint8_t i = 0;

  while (value > 0) {
    temp[i++] = (value % 10) + '0';
    value /= 10;
  }

  // Inverter, pois temp = {Unidade, Dezena, ?} 
  uint8_t j = 0;
  while (i > 0) {
    buffer[j++] = temp[--i];
  }

  buffer[j] = '\0';
}

void timerConfig(void) {
  TA0CCTL0 |= CCIE;
  TA0CTL = TASSEL__SMCLK | MC__UP | TACLR; // @ 1Mhz
  TA0CCR0 = 1000; // 1 ms 
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void) {
  ms++;
}
