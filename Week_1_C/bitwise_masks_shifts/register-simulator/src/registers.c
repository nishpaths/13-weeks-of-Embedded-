#include "registers.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

uint8_t PORTA = 0b00000000;
uint8_t DDRA  = 0b00000000;
uint8_t PINA  = 0b00000000;

static int validBit(int bit)
{
    return bit >= 0 && bit <= 7;
}

void setBit(uint8_t *reg, int bit)
{
    if (!validBit(bit))
        return;
    *reg |= (uint8_t)(1u << bit);
}

void clearBit(uint8_t *reg, int bit)
{
    if (!validBit(bit))
        return;
    *reg &= (uint8_t)~(1u << bit);
}

void toggleBit(uint8_t *reg, int bit)
{
    if (!validBit(bit))
        return;
    *reg ^= (uint8_t)(1u << bit);
}

int checkBit(uint8_t reg, int bit)
{
    if (!validBit(bit))
        return -1;
    return (reg & (uint8_t)(1u << bit)) ? 1 : 0;
}

void printRegisterBinary(const char *name, uint8_t reg)
{
    printf("%s: ", name);
    for (int bit = 7; bit >= 0; bit--)
        printf("%d", checkBit(reg, bit));
    printf("\n");
}

void printBitStates(uint8_t reg)
{
    for (int bit = 0; bit < 8; bit++)
        printf("Bit %d = %s\n", bit, checkBit(reg, bit) ? "ON" : "OFF");
}

void printLEDs(uint8_t porta)
{
    for (int led = 0; led < 8; led++)
        printf("LED%d %s\n", led, checkBit(porta, led) ? "ON" : "OFF");
}

void printAllRegisters(void)
{
    printRegisterBinary("PORTA", PORTA);
    printRegisterBinary("DDRA",  DDRA);
    printRegisterBinary("PINA",  PINA);
    printf("\n");
    printBitStates(PORTA);
    printf("\n");
    printLEDs(PORTA);
}

static uint8_t *selectRegister(const char *name)
{
    if (strcmp(name, "PORTA") == 0) return &PORTA;
    if (strcmp(name, "DDRA")  == 0) return &DDRA;
    if (strcmp(name, "PINA")  == 0) return &PINA;
    return NULL;
}

static void toUpper(char *s)
{
    for (; *s; s++)
        *s = (char)toupper((unsigned char)*s);
}

int processCommand(const char *line)
{
    char cmd[16] = {0};
    char reg_name[8] = {0};
    int bit = -1;

    if (sscanf(line, "%15s %7s %d", cmd, reg_name, &bit) < 1)
        return 1;

    toUpper(cmd);

    if (strcmp(cmd, "QUIT") == 0 || strcmp(cmd, "EXIT") == 0)
        return 0;

    if (strcmp(cmd, "HELP") == 0) {
        printf("Commands:\n");
        printf("  SET <bit>              Set bit on PORTA\n");
        printf("  CLEAR <bit>            Clear bit on PORTA\n");
        printf("  TOGGLE <bit>           Toggle bit on PORTA\n");
        printf("  CHECK <bit>            Read bit on PORTA\n");
        printf("  SET <REG> <bit>        Set bit on PORTA/DDRA/PINA\n");
        printf("  CLEAR <REG> <bit>      Clear bit on PORTA/DDRA/PINA\n");
        printf("  TOGGLE <REG> <bit>     Toggle bit on PORTA/DDRA/PINA\n");
        printf("  CHECK <REG> <bit>      Read bit on PORTA/DDRA/PINA\n");
        printf("  OUTPUT <bit>           Make pin output (set DDRA bit)\n");
        printf("  INPUT <bit>            Make pin input (clear DDRA bit)\n");
        printf("  SHOW                   Display all registers\n");
        printf("  HELP                   Show this message\n");
        printf("  QUIT                   Exit simulator\n");
        return 1;
    }

    if (strcmp(cmd, "SHOW") == 0) {
        printAllRegisters();
        return 1;
    }

    /* Two-token form: SET PORTA 3 */
    uint8_t *reg = selectRegister(reg_name);
    if (reg != NULL && validBit(bit)) {
        if (strcmp(cmd, "SET") == 0) {
            setBit(reg, bit);
            printf("Set %s bit %d\n", reg_name, bit);
        } else if (strcmp(cmd, "CLEAR") == 0) {
            clearBit(reg, bit);
            printf("Cleared %s bit %d\n", reg_name, bit);
        } else if (strcmp(cmd, "TOGGLE") == 0) {
            toggleBit(reg, bit);
            printf("Toggled %s bit %d\n", reg_name, bit);
        } else if (strcmp(cmd, "CHECK") == 0) {
            int state = checkBit(*reg, bit);
            printf("%s bit %d = %s\n", reg_name, bit, state ? "ON" : "OFF");
        } else {
            printf("Unknown command: %s\n", cmd);
        }
        return 1;
    }

    /* One-token form: SET 3 (defaults to PORTA) */
    if (sscanf(line, "%15s %d", cmd, &bit) == 2 && validBit(bit)) {
        toUpper(cmd);
        if (strcmp(cmd, "SET") == 0) {
            setBit(&PORTA, bit);
            printf("Set PORTA bit %d\n", bit);
        } else if (strcmp(cmd, "CLEAR") == 0) {
            clearBit(&PORTA, bit);
            printf("Cleared PORTA bit %d\n", bit);
        } else if (strcmp(cmd, "TOGGLE") == 0) {
            toggleBit(&PORTA, bit);
            printf("Toggled PORTA bit %d\n", bit);
        } else if (strcmp(cmd, "CHECK") == 0) {
            int state = checkBit(PORTA, bit);
            printf("PORTA bit %d = %s\n", bit, state ? "ON" : "OFF");
        } else if (strcmp(cmd, "OUTPUT") == 0) {
            setBit(&DDRA, bit);
            printf("Pin %d set to OUTPUT (DDRA bit %d = 1)\n", bit, bit);
        } else if (strcmp(cmd, "INPUT") == 0) {
            clearBit(&DDRA, bit);
            printf("Pin %d set to INPUT (DDRA bit %d = 0)\n", bit, bit);
        } else {
            printf("Unknown command: %s\n", cmd);
        }
        return 1;
    }

    printf("Unknown or invalid command. Type HELP.\n");
    return 1;
}
