// compile: gcc main.c -o main
// run: ./main


#include <stdio.h>
#include <stdint.h>

typedef enum {
    LED_ENABLE   = 0,
    MOTOR_ENABLE = 1,
    ERROR_FLAG   = 2,
    TX_READY     = 3,
    RX_READY     = 4,
    POWER_SAVE   = 5,
    INTERRUPT_EN = 6,
    SYSTEM_READY = 7
} RegisterBit;

typedef enum{
    READ_WRITE = 0,
    READ_ONLY = 1,
    WRITE_ONLY = 2,
}ACCESS_TYPE;

typedef struct{
    uint8_t value;
    ACCESS_TYPE access_type;
    char name[50];
}Register;

typedef struct{
    Register PORTA;
    Register DDRB;
    Register STATUS;
    Register CMD;
}MMIO_System;


void read();
void write(uint8_t value, Register *reg);

void set_bit(Register *reg, RegisterBit bit);
void clear_bit(Register *reg, RegisterBit bit);
uint8_t read_bit(Register *reg, RegisterBit bit);
void write_mask(Register*reg, uint8_t mask);
void print_reg(Register *reg);

void set_bit(Register *reg, RegisterBit bit){

    if (bit < LED_ENABLE || bit > SYSTEM_READY){
        printf("Invalid bit number\n");
        return;
    }
    reg->value |= (1 << bit);
}
void clear_bit(Register *reg, RegisterBit bit){
    if (bit < LED_ENABLE || bit > SYSTEM_READY){
        printf("Invalid bit number\n");
        return;
    }
    reg->value &= ~(1 << bit);
}
uint8_t read_bit(Register *reg, RegisterBit bit){
    if (bit < LED_ENABLE || bit > SYSTEM_READY){
        printf("Invalid bit number\n");
        return 0;
    }
    return (reg->value & (1 << bit)) ? 1 : 0;
}
void write_mask(Register *reg, uint8_t mask){
    reg->value = mask;
}
void print_reg(Register *reg){
    printf("Register: 0b");

    for(int i = 7; i >= 0; i--){
        printf("%d", (reg->value >> i) & 1);
    }

    printf("\n");
}

int main(){
    MMIO_System mcu ={
        .PORTA = {0x00, READ_WRITE, "PORTA"},
        .DDRB = {0x00, READ_WRITE, "DDRB"},
        .STATUS = {0x00, READ_ONLY, "STATUS"},
        .CMD = {0x00, WRITE_ONLY, "CMD"},
    };
    print_reg(&mcu.PORTA);
    print_reg(&mcu.DDRB);
    print_reg(&mcu.STATUS);
    print_reg(&mcu.CMD);
    return 0;
}


