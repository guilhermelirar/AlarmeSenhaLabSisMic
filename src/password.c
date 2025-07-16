#include <stddef.h>
#include <msp430.h>
#include "password.h"
#include "input.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>

#define FLASH_ADDR 0x1800                   // Information Memory A Adress

u16 admin_password[PASSWORD_SIZE] = {0,0,1,1,0};
uint8_t i;

// Se tiver vazio na memória ele atualiza para a senha admin.
void password_Init(){
    uint16_t* senha_atual = get_password();
    if(senha_atual[0]== 0xFFFF){
        set_password(admin_password);
        }
    }

// Verificia se a senha na memória é igual a senha introduzida 
int check_password(){
    u16* user_password = get_password();                    // resgata a senha na memoria
    volatile u8* entered_password = inputBuffer();        // resgata a senha do buffer de entrada dos botoes

    for (i=0;i<PASSWORD_SIZE;i++){
        u8 flash_byte = user_password[i];
        u8 user_byte = entered_password[i];
        if (flash_byte != user_byte){return 0;}
    }
    return 1;
}

void erase_password(){
    WDTCTL = WDTPW | WDTHOLD;

    while (FCTL3 & BUSY);
    
    FCTL3 = FWPW;
    FCTL1 = FWPW | ERASE ;                  //Segment Erase
    
    *((int16_t*)FLASH_ADDR) = 0xFFFF;               // Escreve vazio   

    while (FCTL3 & BUSY);
    FCTL1 = FWPW;
    FCTL3 = FWPW | LOCK;
}

void write_password(u16 *password){
    uint16_t *ptr = (uint16_t*)FLASH_ADDR;

    WDTCTL = WDTPW | WDTHOLD;
    FCTL3 = FWPW;
    FCTL1 = FWPW | WRT;

    for (i = 0; i < PASSWORD_SIZE; i++) {
        *ptr=password[i];
        while(FCTL3 & BUSY);
        ptr++;
    }

    FCTL1 = FWPW;
    FCTL3 = FWPW | LOCK;
}

void set_password(u16 *password){
    erase_password();
    write_password(password);
}

u16* get_password(){
    return FLASH_ADDR;
}
