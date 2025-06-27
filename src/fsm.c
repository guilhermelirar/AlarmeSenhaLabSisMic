#include "fsm.h"
#include "lcd.h"
#include "utils.h"
#include "input.h"

static volatile State state = SLEEPING;

void updateState() {
  static uint32_t last_input = 0;

  switch (state) {
  case SLEEPING: {
    // Algum botão foi pressionado
    if (getInputSize()) {
      lcdWake();
      lcdWrite("INSIRA A SENHA\n");
      last_input = milis();
      state = READING_INPUT;
    }

    break;
  }

  case READING_INPUT: {
    u8 input_size = getInputSize();
    
    // Entrada pronta para ser verificada
    if (input_size == 5) {
      state = ACCESS_GRANTED;
      lcdWrite("Acesso Liberado!");
    }

    // Tempo desde que a entrada não está vazia
    if (input_size > 0) {
      last_input = milis();  
    } else if (milis() - last_input > 3000) {
      state = SLEEPING; // Entrar no modo dormir
    } 

    break;
  }
  }
}
