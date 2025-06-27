#ifndef INPUT_H
#define INPUT_H

#include <msp430.h>

#define S1 BIT1
#define S2 BIT1

typedef unsigned char u8;

void inputConfig(); // Configura botões
u8 getInputSize(); // 0, 1, 2, 3, 4, 5 
u8* getInput();    // Rentorna entrada 

#endif // INPUT_H
