#ifndef INPUT_H
#define INPUT_H

typedef unsigned char u8;

uint32_t inputEmptySince();
u8 getInputSize(); // 0, 1, 2, 3, 4, 5 
u8* getInput();    // Rentorna entrada 

#endif // INPUT_H
