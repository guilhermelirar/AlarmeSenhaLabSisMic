#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

typedef enum {
    SLEEPING,
    READING_INPUT,
    ACCESS_GRANTED,
    ACCESS_DENIED,
    BLOCKED,
    PASSWORD_CHANGE,
    PASSWORD_CONFIRM
} State;

void updateState();                     // Atualizar o estado da máquina
void stateTransition(State next_state); // Imediatamente após mudança de estado

// Funções para comportamento de cada estado
State stateReadingInput();
State stateAccessGranted();
State stateDenied();
State stateBlocked();

#endif