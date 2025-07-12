#ifndef PASSWORD_H_INCLUDED
#define PASSWORD_H_INCLUDED

#include "utils.h"
#define PASSWORD_SIZE 5

void password_Init();                   // Inicia o FLASH MEMORY CONTROLLER
int check_password();                   // Valida a equivalencia da senha 
void erase_password();                  // Apaga a senha da memoria
void write_password(u8*password);       // Escreve a senha na memoria
void set_password(u8*password);         // Atualiza a senha na memoria
u8* get_password();                     // Acessa a senha na memoria

#endif //PASSWORD_H_INCLUDED
