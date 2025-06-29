#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

// Converte uint8_t para uma string ASCII
// value precisa ser < 100
void uitoascii(u8 value, char *buffer);

// Configuração de timers
void timerConfig();

// Retorna milissegundos que passaram
u32 milis(void);

// Retorna 1 se o tempo expirou, 0 caso contrário
u8 timeout(u32 start, u16 limit);

#endif // UTILS_H_INCLUDED
