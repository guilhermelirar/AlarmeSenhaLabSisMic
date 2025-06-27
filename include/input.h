#ifndef INPUT_H
#define INPUT_H

#include <msp430.h>
#include <stdint.h>

typedef unsigned char u8;

typedef struct {
  u8 buffer[5];
  u8 size;
  uint32_t last_input_time;
} Input;

static volatile Input input;

Input* getInput();  // Retorna entrada 

#endif // INPUT_H
