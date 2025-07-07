#include "command.h"

u8 handleCommand(char* cmd) {
    if (cmd[0] == 'S' && cmd[1] == 'E' && cmd[2] == 'T' &&
          cmd[3] == 'P' && cmd[4] == 'W' && cmd[5] == ' ')
    {
        u8 new_pw[5];
        // Lê os 5 bits
        u8 i;
        for (i = 5; i; i--) {
            char c = cmd[6 + i];
            if (c != '0' && c != '1')
                return 0; // Inválido

            new_pw[i] = c - '0'; // Converte '0' ou '1' para 0 ou 1

            // TODO: SET PASSWORD
        }

        return 1;
    }

}
