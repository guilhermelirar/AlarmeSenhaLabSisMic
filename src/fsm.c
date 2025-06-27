#include "fsm.h"
#include "lcd.h"
#include "utils.h"
#include "input.h"

void updateState() {
  static volatile State state = SLEEPING;
  static Input input = getInput();

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
    // Entrada pronta para ser verificada
    if (input.size == 5) {
      input.size = 0;
      state = ACCESS_GRANTED;
      lcdWrite("Acesso Liberado!");
    }

    // Tempo desde que a entrada não está vazia
    if (input.size == 0 && (milis() - input.last_input_time) > 3000) {
      state = SLEEPING; // Entrar no modo dormir
    } 

    break;
  }

  case ACCESS_GRANTED: {
      
    break;
  }
  }
}
