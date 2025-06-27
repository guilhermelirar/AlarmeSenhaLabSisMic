#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

#define LED_R BIT0
#define LED_G BIT7

#include "utils.h"

uint32_t ms;

// Inicialização das Configurações dos Leds
void led_Init();

// Ligar LED VERMELHO
void led_R_on();

// DESLIGAR LED VERMELHO
void led_R_off();

// LIGAR LED VERDE
void led_G_on();

//DESLIGAR LED VERDE
void led_G_off();

// Configuraçao do LED no modo sleeping
void led_Sleeping_Mode();

//  Configuracao do LED no modo de leitura
void led_Reading_Input();

// Configuracao do LED quando acesso garantido
void led_Acess_Garanted();

// Configuracao do LED quando acesso negado
void led_Acess_Denied();

// Configuracao do LED quando dispositivo bloqueado
void led_Blocked();

// Configuracao do Led vermelho comecar a piscar
void led_R_stt_Blink();

// Configuracao do Led vermelho parar de piscar
void led_R_stp_Blink();

// Configuracao do Led verde comecar a piscar
void led_G_stt_Blink();

// Configuracao do Led verde parar de piscar
void led_G_stp_Blink();

#endif //LED_H_INCLUDED