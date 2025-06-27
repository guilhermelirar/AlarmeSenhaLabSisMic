#include "include/led.h"
#include "utils.h"
#include <msp430.h>

uint32_t ms = milis();

void led_Init(){
    P1DIR |= LED_R;     // Define o Led vermelho como saida
    P4DIR |= LED_G;     // Define o Led verde como saida

    led_R_off();        // Garante que o Led vermelho comece desligado
    led_G_off();        // Garante que o Led verde comece desligado
}

void led_R_on(){
    P1OUT |= LED_R;     // Liga o LED vermelho
}

void led_R_off(){
    P1OUT &= ~LED_R;    // Desliga o LED vermelho
}

void led_G_on(){
    P4OUT |= LED_G;     // Liga o LED verde
}

void led_G_off(){
    P4OUT &= ~LED_G;    // Desliga o LED verde
}

void led_Sleeping_Mode(){
    led_R_stp_Blink();
    led_G_stp_Blink();
    led_R_on();
    led_G_off();
}
void led_Reading_Input(){
    led_R_stp_Blink();
    led_G_stp_Blink();

}

void led_Acess_Garanted(){
    led_R_stp_Blink();
    led_R_off();
    led_G_stt_Blink();
}
void led_Acess_Denied(){
    led_R_stt_Blink();
    led_G_stp_Blink();
    led_G_off();
}

void led_Blocked(){
    led_R_stt_Blink();
    led_G_stp_Blink();
    led_G_off();
}

void led_R_stt_Blink(){
    led_R_on();
    if(ms- milis() >= 100){
        P1OUT^=LED_R;
        ms = milis();
    }
}

void led_R_stp_Blink(){
    led_R_off();
}

void led_G_stt_Blink(){
    led_G_on();
    if(milis()- ms >= 1000){
        P4OUT^=LED_G;
        ms = milis();
    }
}

void led_G_stp_Blink(){
    led_G_off();
}
