#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

#include <stdint.h>

#define LCD_I2C_ADDR 0x27 // ou 0x3F

#define LCD_RS 0x01 // 0: Comando, 1: Dados
#define LCD_RW 0x02 // 0: Escrita, 1: Leitura
#define LCD_EN 0x04 // Pulso de habilitação
#define LCD_BL 0x08 // Controle da luz de fundo

// Configura os pinos do LCD e inicializa o I2C e o LCD
void lcdPreInit(void);

// Inicializa o I2C no modo mestre
void i2cInitMaster(void);

void lcdInit(void);

// Envia um byte para o dispositivo I2C indicado pelo endereço
uint8_t i2cSend(uint8_t slaveAddr, uint8_t data);

// Envia 4 bits (nibble) para o LCD. isChar=1 para caractere, 0 para comando.
uint8_t lcdWriteNibble(uint8_t nibble, uint8_t isChar);

// Envia 1 byte para o LCD (como dois nibbles). isChar=1 para caractere, 0 para
// comando.
uint8_t lcdWriteByte(uint8_t byte, uint8_t isChar);

// Escreve uma string no LCD
void lcdWrite(char *str);

// Escreve mensagem de aguardar remainingSeconds
void lcdWriteWaitSeconds(uint8_t remainingSeconds);

static inline void lcdSleep(void) {
  lcdWriteByte(0x08, 0); // Display OFF, cursor OFF, blink OFF
}

static inline void lcdWake(void) {
  lcdWriteByte(0x0C, 0); // Display ON, cursor OFF, blink OFF
}

static inline void lcdClear(void) {
  lcdWriteByte(1, 0);    // Limpar display
  __delay_cycles(20000); // Blind wait
}

#endif // LCD_H_INCLUDED
