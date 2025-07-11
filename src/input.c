#include <msp430.h>
#include "input.h"

static volatile Input input;

void inputInit(void)
{
    // Configuração botões P1.1 e P2.1

    // Entrada
    P1DIR &= ~BIT1;   
    P2DIR &= ~BIT1;

    // Resistor interno
    P1REN |= BIT1;
    P2REN |= BIT1;
    P1OUT |= BIT1; // pull-up
    P2OUT |= BIT1; // --

    // Configuração de timer para debounce
    TA1CCTL0 = CCIE;
    TA1CCR0 = 5000;
    TA1CTL = TASSEL_2 + MC_0 + TACLR;
}

void clearInput(void)
{
    input.length = 0;
    input.last_modified = milis();
}

void inputUpdate(void)
{
    inputDisable();
    if (input.length && timeout(input.last_modified, 3000))
    {
        clearInput();
    }
    inputEnable();
}

void inputEnable(void)
{
    // Interrupções na borda de descida dos botões
    P1IES |= BIT1;
    P1IFG &= ~BIT1;
    P1IE  |= BIT1;
    
    P2IES |= BIT1;
    P2IFG &= ~BIT1;
    P2IE  |= BIT1;
}

void inputDisable(void)
{
    P2IE &= ~BIT1;
    P1IE &= ~BIT1;
}

volatile u8* inputBuffer(void)
{
    return input.buffer;
}

u8 inputIsActive(void)
{
    return !(input.length == 0 && timeout(input.last_modified, 3000));
}

u8 inputLength(void)
{
    return input.length;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void debounce(void) {
    TA1CTL = MC_0;    // Para o timer

    // Evitar overflow de buffer
    if (input.length >= PASSWORD_SIZE) return;

    inputEnable();    // Reabilita interrupções

    // Botão ainda pressionado? registrar evento
    // (else if para invalidar quando dois botões pressionados)
    if ((P1IN & BIT1) == 0) 
    {
        input.buffer[input.length++] = 2;  
        input.last_modified = milis();
    } 
    else if ((P2IN & BIT1) == 0) 
    {    
        input.buffer[input.length++] = 1;  
        input.last_modified = milis();
    }
}

#pragma vector=PORT2_VECTOR
__interrupt void S1_ISR(void) {
  if (P2IFG & BIT1) {
    inputDisable();
    P2IFG &= ~BIT1;

    // Timer de debounce
    TA1CTL = TASSEL_2 + MC_1 + TACLR;
  }
}

#pragma vector=PORT1_VECTOR
__interrupt void S2_ISR(void) {
  if (P1IFG & BIT1) {
    inputDisable();
    P1IFG &= ~BIT1;

    TA1CTL = TASSEL_2 + MC_1 + TACLR;
  }
}