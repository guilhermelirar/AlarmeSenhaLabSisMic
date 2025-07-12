#ifndef UART_H
#define UART_H

#include <msp430.h>

typedef unsigned char u8;
typedef unsigned int u16;

// Pinos P4.4 - TX
//       P4.5 - RX
// Fb = 9600
// 1) BRW = 2^20/9600 = 109,23
//    BRW = 109
//    BRS = 0,23 x 8 = 1,8 => 2

void uartPrint(char * string);

void uartConfig();

u8 commandPending();

u8* getCommand();

#endif // UART_H
