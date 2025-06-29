
#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

#define LED_R BIT0
#define LED_G BIT7

#include "utils.h"

// Inicialização das Configurações dos Leds
void led_Init();

// Ligar e desligar LED VERMELHO
void led_R_on();
void led_R_off();

// Ligar e desligar LED VERDE
void led_G_on();
void led_G_off();

// Alternar LEDS
void led_G_toggle();
void led_R_toggle();

// Modos de operação
void led_Sleeping_Mode();
void led_Reading_Input();
void led_Acess_Garanted();
void led_Acess_Denied();
void led_Blocked();

// Piscamento de LEDs
void led_R_stt_Blink(u32 period);
void led_G_stt_Blink(u32 period);
void led_update();

#endif // LED_H_INCLUDED