#include "fsm.h"
#include "input.h"
#include "lcd.h"
#include "led.h"
#include "uart.h"

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

            __low_power_mode_3(); // trocar p 0 se bugar
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
        if (input[0] == 1 && input[1] == 1 &&
            input[2] == 1 && input[3] == 1 &&
            input[4] == 1) {
            // Ir para acesso garantido
            clearInput();
            return ACCESS_GRANTED;
        } else {
            clearInput();
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
        access_attempts = 0;
        lcdClear();
        lcdWrite("ACESSO LIBERADO!");
        led_G_on();
        uartPrint("ACESSO LIBERADO\n");
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
        uartPrint("ACESSO NEGADO\n");
        led_G_off();
        led_R_stt_Blink(500);
        access_attempts++;
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
        access_attempts = 0;
        inputDisable();
        lcdClear();
        led_R_stt_Blink(500);
        last_time_dec = milis();
        time_remaining = 10;
        uartPrint("BLOQUEADO APÓS 3 TENTATIVAS\n");
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
