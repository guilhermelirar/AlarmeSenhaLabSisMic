#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdint.h>

// Converte uint8_t para uma string ASCII
// value precisa ser < 100
void uitoascii(uint8_t value, char *buffer);

// Configuração de timers
void timerConfig();

// Retorna milissegundos que passaram
uint32_t milis(void); 

#endif // UTILS_H_INCLUDED
