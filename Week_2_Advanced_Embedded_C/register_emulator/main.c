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


void set_bit(uint8_t *reg, RegisterBit bit);
void clear_bit(uint8_t *reg, RegisterBit bit);
uint8_t read_bit(uint8_t reg, RegisterBit bit);
void write_mask(uint8_t *reg, uint8_t mask);
void print_reg(uint8_t reg);

void set_bit(uint8_t *reg, RegisterBit bit){

    if (bit < LED_ENABLE || bit > SYSTEM_READY){
        printf("Invalid bit number\n");
        return;
    }
    *reg |= (1 << bit);
}
void clear_bit(uint8_t *reg, RegisterBit bit){
    if (bit < LED_ENABLE || bit > SYSTEM_READY){
        printf("Invalid bit number\n");
        return;
    }
    *reg &= ~(1 << bit);
}
uint8_t read_bit(uint8_t reg, RegisterBit bit){
    if (bit < LED_ENABLE || bit > SYSTEM_READY){
        printf("Invalid bit number\n");
        return 0;
    }
    return (reg & (1 << bit)) ? 1 : 0;
}
void write_mask(uint8_t *reg, uint8_t mask){
    *reg = mask;
}
void print_reg(uint8_t reg){
    printf("Register: 0b");

    for(int i = 7; i >= 0; i--){
        printf("%d", (reg >> i) & 1);
    }

    printf("\n");
}

int main(){
    uint8_t REG = 0b10101010;
    printf("MOTOR_ENABLE: %d\n", read_bit(REG, MOTOR_ENABLE));
    printf("LED_ENABLE: %d\n", read_bit(REG, LED_ENABLE));
    printf("SYSTEM_READY: %d\n", read_bit(REG, SYSTEM_READY));
}


