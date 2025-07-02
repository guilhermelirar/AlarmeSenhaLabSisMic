#include "led.h"
#include "utils.h"
#include <msp430.h>

u8 blinking = 0;
u32 led_r_period = 0, led_g_period = 0, led_r_start = 0, led_g_start = 0;

void led_Init() {
  P1DIR |= LED_R; // Define o Led vermelho como saida
  P4DIR |= LED_G; // Define o Led verde como saida

  led_R_off(); // Garante que o Led vermelho comece desligado
  led_G_off(); // Garante que o Led verde comece desligado
}

void led_R_on() {
  blinking &= ~LED_R; // Para de piscar
  P1OUT |= LED_R;     // Liga o LED vermelho
}

void led_R_off() {
  blinking &= ~LED_R; // Para de piscar
  P1OUT &= ~LED_R;    // Desliga o LED vermelho
  led_r_period = 0;
}

void led_G_on() {
  blinking &= ~LED_G; // Para de piscar
  P4OUT |= LED_G;     // Liga o LED verde
  led_g_period = 0;
}

void led_G_off() {
  blinking &= ~LED_G; // Para de piscar
  P4OUT &= ~LED_G;    // Desliga o LED verde
}

void led_G_toggle() {
  P4OUT ^= LED_G; // Alterna estado
}

void led_R_toggle() {
  P1OUT ^= LED_R; // Alterna estado
}

void led_Sleeping_Mode() {
  led_G_off();
  led_R_on();
}

void led_Reading_Input() {
  led_G_off();
  led_R_off();
}

void led_Acess_Garanted() {
  led_R_off();
  led_G_stt_Blink(500);
}

void led_Acess_Denied() {
  led_R_stt_Blink(250);
  led_G_off();
}

void led_Blocked() {
  led_R_stt_Blink(500);
  led_G_off();
}

void led_R_stt_Blink(u32 period) {
  led_r_start = milis();  // Pega tempo atual
  led_r_period = period;  // Seta período de alternar
  blinking |= LED_R;      // Sinaliza blinking
}

void led_G_stt_Blink(u32 period) {
  led_g_start = milis();
  led_g_period = period;
  blinking |= LED_G;
}

void led_update() {
  // Se led vermelho pisca
  if ((blinking & LED_R)) {
    // Passou período de piscar
    if (timeout(led_r_start, led_r_period)) {
      led_R_toggle();         // Alterna estado
      led_r_start = milis();  // Novo referencial
    }
  }

  // Se led verde pisca
  if ((blinking & LED_G)) {
    // Passou período de piscar
    if (timeout(led_g_start, led_g_period)) {
      led_G_toggle();         // Alterna estado
      led_g_start = milis();  // Novo referencial
    }
  } 
}
