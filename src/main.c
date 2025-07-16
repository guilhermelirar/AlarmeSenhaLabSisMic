#include <msp430.h>
#include "lcd.h"
#include "led.h"
#include "utils.h"
#include "fsm.h"
#include "input.h"
#include "password.h"
#include "uart.h"

u8 handleCommand(u8 *cmd);

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

  // Inicialização de módulos
  erase_password();
  lcdPreInit();
  led_Init();
  inputInit();
  timerConfig();

  password_Init();

  uartConfig();

        
  // Interrupções
  __enable_interrupt();

  while(1)                                  // continuous loop
  {
    inputUpdate();   
    updateState();
    led_update();
    
    // Verifica comandos UART a cada laço
    if (commandPending()) 
    {
      if (handleCommand(getCommand())) 
        uartPrint("Comando executado com sucesso!\n");
      else
        uartPrint("O comando passado era inválido!\n"); 
    }
  }
}

u8 handleCommand(u8* cmd) {
    if (cmd[0] == 'S' && cmd[1] == 'E' && cmd[2] == 'T' &&
          cmd[3] == 'P' && cmd[4] == 'W' && cmd[5] == ' ')
    {
        u8 new_pw[5];
        // Lê os 5 bits
        u8 i;
        for (i = 5; i; i--) {
            char c = cmd[6 + i];
            if (c != '0' && c != '1')
                return 0; // Inválido

            new_pw[i] = c - '0'; // Converte '0' ou '1' para 0 ou 1

            // TODO: SET PASSWORD
        }

        return 1;
    }

}
