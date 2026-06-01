/* compile: gcc src/main.c src/registers.c -o register-simulator */
/* run:     ./register-simulator */

#include "registers.h"

#include <stdio.h>

int main(void)
{
    char line[128];

    printf("Virtual Register Simulator\n");
    printf("Type HELP for commands, QUIT to exit.\n\n");
    printAllRegisters();
    printf("\n> ");

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (processCommand(line) == 0)
            break;
        printf("\n> ");
    }

    printf("Goodbye.\n");
    return 0;
}
