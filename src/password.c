#include <stddef.h>
#include <msp430.h>
#include "password.h"
#include "input.h"
#include <stdint.h>
#define FLASH_ADDR 0x1800                   // Information Memory A Adress

u8 admin_password[PASSWORD_SIZE] = {0,0,0,1,0};
uint8_t i;

// Se tiver vazio na memória ele atualiza para a senha admin.
void password_Init(){
    u8* ptr;
    ptr = get_password();
    if (ptr[0]==0xFF){                     
        set_password(admin_password);
    }
}

// Verificia se a senha na memória é igual a senha introduzida 
int check_password(){
    u8* user_password = get_password();          // resgata a senha na memoria
    volatile u8* entered_password = inputBuffer();        // resgata a senha do buffer de entrada dos botoes

    for (i=0;i<PASSWORD_SIZE;i++){
        if(user_password[i]!=entered_password[i]){
            return 1;
        }
    }
    return 0;
}

void erase_password(){
    WDTCTL = WDTPW | WDTHOLD;

    while (FCTL3 & BUSY);

    FCTL3 |= FWPW;
    FCTL1 = FWPW | ERASE ;                  //Segment Erase
    
    int16_t* dummy_write;
    dummy_write = ((int16_t*)FLASH_ADDR);
    *dummy_write = 0xFFFF;                  // Escreve vazio   

    FCTL1 |= FWPW;
    FCTL3 = FWPW | LOCK;
}

void write_password(u8 *password){
    uint16_t *ptr = (uint16_t*)FLASH_ADDR;

    WDTCTL = WDTPW | WDTHOLD;
    FCTL3 |= FWPW;
    FCTL1 = FWPW | WRT;

    for (i = 0; i < PASSWORD_SIZE; i++) {
        *(ptr + i)  = password[i];                  

        while(FCTL3 & BUSY);

    }

    FCTL1 |= FWPW;
    FCTL3 = FWPW | LOCK;
}

void set_password(u8 *password){
    erase_password();
    write_password(password);
}

u8* get_password(){
    u8* user_password;
    user_password = (u8 *)FLASH_ADDR;
    return user_password;
}
