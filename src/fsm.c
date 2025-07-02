#include "fsm.h"
#include "input.h"
#include "lcd.h"
#include "led.h"

u32 last_transition;
u8 access_attempts = 0;

void updateState(void)
{
    // Para detectar transições de estado
    static State previous_state = BLOCKED;
    static State state = SLEEPING;

    u8 entering = previous_state != state;
    previous_state = state;

    if (entering)
    {
        last_transition = milis();
    }

    // Usar funções de comportamento de cada estado
    switch (state) {
        case (SLEEPING):
        {
            if (entering) 
            {
                lcdSleep();
                led_G_off();
                led_R_stt_Blink(500);
            }
            if (inputLength()) state = READING_INPUT;
            break;
        }
        case (READING_INPUT):
        {
            state = stateReadingInput(entering);
            break;
        }
        case (ACCESS_GRANTED):
        {
            state = stateAccessGranted(entering);
            break;
        }
        case (ACCESS_DENIED):
        {
            state = stateAccessDenied(entering);
            break;
        }
        default: {
            state = SLEEPING;
            break;
        }
    }
}

State stateReadingInput(u8 entering)
{
    if (entering)
    {
        lcdClear();
        led_R_off();
        lcdWrite("INSIRA A SENHA:");
        inputEnable();
    }

    // 3 segundos com entrada vazia
    if (!inputIsActive()) {
        return SLEEPING;
    };

    u8 input_length = inputLength();

    // Senha completa
    if (input_length == 5)
    {
        volatile u8* input = inputBuffer();
        
        // ==== TODO: checar senha real ====== 
        if (input[0] == 1 && input[1] == 2 &&
            input[2] == 1 && input[3] == 2 &&
            input[4] == 1) {
            // Ir para acesso garantido
            access_attempts = 0;
            return ACCESS_GRANTED;
        } else {
            access_attempts++;
            return ACCESS_DENIED;
        }
        // ==================================
    }
    return READING_INPUT;
}

State stateAccessGranted(u8 entering)
{
    if (entering)
    {
        lcdClear();
        lcdWrite("ACESSO LIBERADO!");
        led_G_on();
    }

    if (timeout(last_transition, 3000))
        return SLEEPING;

    return ACCESS_GRANTED;
}

State stateAccessDenied(u8 entering)
{
    if (entering) 
    {
        inputDisable();
        lcdClear();
        lcdWrite("ACESSO NEGADO!");
        led_G_off();
        led_R_stt_Blink(500);
    }

    // Depende de quantas tentativas erradas seguidas
    if (timeout(last_transition, 3000))
    {
        return access_attempts < 3 ?
            READING_INPUT : BLOCKED;
    }

    return ACCESS_DENIED;
}

State stateBlocked(u8 entering)
{
    static u8 time_remaining = 10;
    static u32 last_time_dec = 0;
    if (entering)
    {
        inputDisable();
        lcdClear();
        led_R_stt_Blink(500);
        last_time_dec = milis();
        time_remaining = 10;
    }
    
    if (timeout(last_time_dec, 1000))
    {
        time_remaining--;
        last_time_dec = milis();
    }

    // Continua no mesmo estado se ainda tem que esperar
    if (time_remaining)
    {
        lcdWriteWaitSeconds(time_remaining);
        return BLOCKED;
    }

    // Pode receber entradas agora
    return READING_INPUT;
}
