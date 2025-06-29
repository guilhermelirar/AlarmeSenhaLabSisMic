#ifndef INPUT_H
#define INPUT_H

#define PASSWORD_SIZE 5

#include "utils.h"

typedef struct {
  u8 buffer[PASSWORD_SIZE]; // Dígitos da senha
  u8 length;                // Qtd dígitos inseridos
  u32 last_modified;        // Tempo desde última modificação no buffer
} Input;

void inputInit();           // Configuração dos botões e timers
void inputUpdate();         // Atualiza input conforme regras de timeout
void inputEnable();         // Ativa interrupção de botão
void inputDisable();        // Desativa interrupções de botão
volatile u8* inputBuffer(); // Retorna o buffer com os dígitos inseridos
u8 inputIsActive();         // Retorna 1 se input não foi vazio por mais de 3s, 0 caso contrário
u8 inputLength();           // Retorna qtd dígitos inseridos 

#endif // INPUT_H