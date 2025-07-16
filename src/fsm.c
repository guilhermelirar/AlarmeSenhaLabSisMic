#include "fsm.h"
#include "input.h"
#include "lcd.h"
#include "led.h"
#include "uart.h"
#include "password.h"

u32 last_transition;
u8 access_attempts = 0;
u8 new_pw[PASSWORD_SIZE];

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
                access_attempts = 0;
                lcdSleep();
                led_G_off();
                led_R_stt_Blink(500);
            }

            //__low_power_mode_3(); // trocar p 0 se bugar
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

    // Senha completa
    if (inputLength() == PASSWORD_SIZE)
    {   
        if (check_password()) {
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

    if (inputLength())
    {
        clearInput();
        return PASSWORD_CHANGE;
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

State statePasswordChange(u8 entering)
{
    if (entering)
    {
        led_G_off();
        
        if (access_attempts) {
            led_R_stt_Blink(500);
        } else {
            led_R_off();
        }

        lcdClear();
        lcdWrite("INSIRA NOVA SENHA\n");
        access_attempts = 0;
        inputEnable();
    }

    // Parar de piscar
    if (timeout(last_transition, 3000))
    {
        led_R_off();
    }


    // 3 segundos com entrada vazia
    if (!inputIsActive()) {
        return SLEEPING;
    };

    // Senha completa
    if (inputLength() == PASSWORD_SIZE)
    {   
        u8 i;
        for (i=0; i<PASSWORD_SIZE; i++) {
            new_pw[i] = inputBuffer()[i];
        }
        return PASSWORD_CONFIRM;
    }

    return PASSWORD_CHANGE;
}

State statePasswordConfirm(u8 entering) 
{   
    if (entering)
    {
        access_attempts = 0;
        led_G_off();
        led_R_off();
        lcdClear();
        lcdWrite("CONFIRME A SENHA\n");
        inputEnable();
    }

    // 3 segundos com entrada vazia
    if (!inputIsActive()) {
        return SLEEPING;
    };

    // Senha completa
    if (inputLength() == PASSWORD_SIZE)
    {   
        u8 i;
        for (i=0; i<PASSWORD_SIZE; i++) {
            if (new_pw[i] != inputBuffer()[i]) 
            {
                clearInput();
                access_attempts++;
                return PASSWORD_CHANGE; // Pede para inserir pela primeira vez
            }
        }
        set_password(new_pw); // Nova senha
        return SLEEPING;
    }

    return PASSWORD_CONFIRM;
}