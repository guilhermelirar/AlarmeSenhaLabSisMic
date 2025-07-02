#include "lcd.h"
#include "utils.h"
#include <msp430.h>

void lcdPreInit(void) {
  // UCB0: P3.0 = SDA, P3.1 = SCL
  P3SEL |= BIT0 | BIT1;    // I2C
  P3DIR &= ~(BIT0 | BIT1); // Saída

  // Comentar em caso de resistores externos
  P3REN |= BIT0 | BIT1; // Resistores
  P3OUT |= BIT0 | BIT1; // ... de pull up

  i2cInitMaster();
  __delay_cycles(200000);
  lcdInit();
}

void i2cInitMaster(void) {
  UCB0CTL1 |= UCSWRST;                  // Reseta para iniciar a configuração
  UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC; // Mestre, I2C (MODE = 3), síncrono
  UCB0CTL1 = UCSSEL_2 | UCSWRST;        // Usa SMCLK (1 MHz), mantém reset
  UCB0BR0 = 10;                         // SMCLK/10 = 100 kHz
  UCB0BR1 = 0;
  UCB0CTL1 &= ~UCSWRST; // Limpa reset para terminar a configuração
}

void lcdInit(void) {
  uint8_t nibble = 3 << 4; // Colocando no msb

  // Iniciando LCD corretamente
  lcdWriteNibble(nibble, 0);
  __delay_cycles(20000);
  lcdWriteNibble(nibble, 0);
  __delay_cycles(20000);
  lcdWriteNibble(nibble, 0);
  __delay_cycles(20000);

  // Modo 4 bits
  nibble = 2 << 4;
  lcdWriteNibble(nibble, 0);
  __delay_cycles(20000);

  lcdClear();
}

uint8_t i2cSend(uint8_t slaveAddr, uint8_t data) {
  UCB0I2CSA = slaveAddr; // USCI B0 I2C Slave Addr
  UCB0CTL1 |= UCTR |     // Modo transmissor
              UCTXSTT;   // Gerar condição START

  while (!(UCB0IFG & UCTXIFG))
    ; // Esperando buffer
  UCB0TXBUF = data;

  while (UCB0CTL1 & UCTXSTT)
    ;                  // Espera o START ser enviado
  UCB0CTL1 |= UCTXSTP; // Gerar condição STOP

  while (UCB0CTL1 & UCTXSTP)
    ; // Espera STOP ser enviado

  return UCB0IFG & UCNACKIFG; // NACK
}

uint8_t lcdWriteNibble(uint8_t nibble, uint8_t isChar) {
  uint8_t data = (nibble) & 0xF0; // Coloca o nibble em D7–D4
  data |= LCD_BL;                 // Mantém a luz de fundo ligada

  if (isChar)
    data |= LCD_RS; // RS = 1 se for caractere

  uint8_t nack = i2cSend(LCD_I2C_ADDR, data);   // EN = 0
  nack |= i2cSend(LCD_I2C_ADDR, data | LCD_EN); // EN = 1 (subida)
  nack |= i2cSend(LCD_I2C_ADDR, data);          // EN = 0 (descida)

  return nack;
}

uint8_t lcdWriteByte(uint8_t byte, uint8_t isChar) {
  uint8_t nack = lcdWriteNibble(byte, isChar); // byte[7:4]
  nack |= lcdWriteNibble(byte << 4, isChar);   // byte[3:0]
  return nack;
}

void lcdWrite(char *str) {
  while (*str) {
    char c = *str++;

    if (c == '\n') {         // Equivale a \n\r para simplificar
      lcdWriteByte(0x80 |    // Set DDRAM Address
                       0x40, // 1a posição da 2a linha
                   0);       // Comando
      continue;
    }

    lcdWriteByte(c, 1);
  }
}

void lcdWriteWaitSeconds(uint8_t remainingSeconds) {
  lcdClear(); // Sobrescrever
  lcdWrite("   Aguarde ");

  char c[3];
  uitoascii(remainingSeconds, c);
  lcdWrite(c);

  lcdWrite(" s...");
}
