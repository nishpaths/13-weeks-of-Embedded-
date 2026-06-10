//compile: gcc dispatcher.c -o dispatcher
//run: ./dispatcher


#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef void (*command_func)(uint8_t *reg);

void cmd_set(uint8_t *reg);
void cmd_enable(uint8_t *reg);
void cmd_clear(uint8_t *reg);


void cmd_set(uint8_t *reg){
    *reg = 0xFF;
    printf("SET command executed successfully \n");
}
void cmd_enable(uint8_t *reg){
    *reg |= 0x01;
    printf("ENABLE command executed successfully \n");
}
void cmd_clear(uint8_t *reg){
    *reg = 0x00;
    printf("CLEAR command executed successfully \n");
}

typedef struct{
    char command_name[50];
    command_func handler;
}CommandCreate;

int main(){
    char input[32];
    uint8_t test = 0x3A;
    CommandCreate commands[] = {
        {"SET", cmd_set}, {"ENABLE", cmd_enable}, {"CLEAR", cmd_clear}
    };

    printf("Enter command: ");
    scanf("%31s", input);
    int found = 1;
    for (int i = 0; i < 3; i++){
        if (strcmp(input, commands[i].command_name) == 0){
            commands[i].handler(&test);
            found = 0;
        }
    }
    if (found == 1){
        printf("No command found \n");
    }
    printf("%u", test);
    return 0;
}
