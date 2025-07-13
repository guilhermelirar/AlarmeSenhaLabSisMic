#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#include "utils.h"

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

// Funções para comportamento de cada estado
//  entering deve ser !0 se entrando no estado pela primeira vez
//  e 0 se não houve mudança de estado
State stateReadingInput(u8 entering);
State stateAccessGranted(u8 entering);
State stateAccessDenied(u8 entering);
State stateBlocked(u8 entering);
State statePasswordChange(u8 entering);
State statePasswordConfirm(u8 entering);

#endif // FINITE_STATE_MACHINE_H
