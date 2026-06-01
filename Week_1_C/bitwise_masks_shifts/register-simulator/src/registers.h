#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

/* AVR-style register names — same roles as on a real microcontroller */
extern uint8_t PORTA;  /* output latch: drives pins high/low */
extern uint8_t DDRA;   /* data direction: 1 = output, 0 = input */
extern uint8_t PINA;   /* pin input state (simulated switches) */

void setBit(uint8_t *reg, int bit);
void clearBit(uint8_t *reg, int bit);
void toggleBit(uint8_t *reg, int bit);
int checkBit(uint8_t reg, int bit);

void printRegisterBinary(const char *name, uint8_t reg);
void printBitStates(uint8_t reg);
void printLEDs(uint8_t porta);
void printAllRegisters(void);

int processCommand(const char *line);

#endif
