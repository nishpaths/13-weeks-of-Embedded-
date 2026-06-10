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


uint8_t read_register(Register *reg);
void write_register(Register *reg, uint8_t value);

void set_bit(Register *reg, RegisterBit bit);
void clear_bit(Register *reg, RegisterBit bit);
uint8_t read_bit(Register *reg, RegisterBit bit);
void write_mask(Register*reg, uint8_t mask);
void print_reg(Register *reg);

void set_bit(Register *reg, RegisterBit bit){

    if (reg->access_type == READ_ONLY){
        printf("Register is read only\n");
        return;
    }
    if (bit < LED_ENABLE || bit > SYSTEM_READY){
        printf("Invalid bit number\n");
        return;
    }
    reg->value |= (1 << bit);
}
void clear_bit(Register *reg, RegisterBit bit){
    if (reg->access_type == READ_ONLY){
        printf("Register is read only\n");
        return;
    }
    if (bit < LED_ENABLE || bit > SYSTEM_READY){
        printf("Invalid bit number\n");
        return;
    }
    reg->value &= ~(1 << bit);
}
uint8_t read_bit(Register *reg, RegisterBit bit){
    if (reg->access_type == WRITE_ONLY){
        printf("Register is write only\n");
        return 0;
    }
    if (bit < LED_ENABLE || bit > SYSTEM_READY){
        printf("Invalid bit number\n");
        return 0;
    }
    return (reg->value & (1 << bit)) ? 1 : 0;
}
void print_reg(Register *reg){
    if (reg->access_type == WRITE_ONLY){
        printf("%s: <write-only register>\n", reg->name);
        return;
    }

    printf("%s: 0b", reg->name);

    for(int i = 7; i >= 0; i--){
        printf("%d", (reg->value >> i) & 1);
    }

    printf("\n");
}

uint8_t read_register(Register *reg){
    if (reg->access_type == WRITE_ONLY){
        printf("Register is write only\n");
        return 0;
    }
    return reg->value;
}

void write_register(Register *reg, uint8_t value){
    if (reg->access_type == READ_ONLY){
        printf("Register is read only\n");
        return;
    }
    reg->value = value;
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

    // test 1
    printf("\nWriting 0xAA to PORTA\n");
    write_register(&mcu.PORTA, 0xAA);
    print_reg(&mcu.PORTA);

    // test 2
    printf("\nTrying to write 0xFF to STATUS\n");
    write_register(&mcu.STATUS, 0xFF);
    print_reg(&mcu.STATUS);

    // test 3
    printf("\nWriting 0x55 to CMD\n");
    write_register(&mcu.CMD, 0x55);
    print_reg(&mcu.CMD);

    // test 4
    printf("\nTrying to read CMD\n");
    uint8_t cmd_value = read_register(&mcu.CMD);
    printf("CMD read returned: %d\n", cmd_value);

    // test 5
    printf("\nTrying to set SYSTEM_READY on STATUS\n");
    set_bit(&mcu.STATUS, SYSTEM_READY);
    print_reg(&mcu.STATUS);

    printf("\nSetting LED_ENABLE on PORTA\n");
    set_bit(&mcu.PORTA, LED_ENABLE);
    print_reg(&mcu.PORTA);

    printf("\nReading STATUS bit SYSTEM_READY\n");
    printf("STATUS SYSTEM_READY: %d\n", read_bit(&mcu.STATUS, SYSTEM_READY));

    printf("\nTrying to read bit from CMD\n");
    printf("CMD LED_ENABLE: %d\n", read_bit(&mcu.CMD, LED_ENABLE));
    return 0;
}


