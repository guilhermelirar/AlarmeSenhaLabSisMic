#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdint.h>

// Converte uint8_t para uma string ASCII
// value precisa ser < 100
void uitoascii(uint8_t value, char *buffer);

#endif // UTILS_H_INCLUDED
