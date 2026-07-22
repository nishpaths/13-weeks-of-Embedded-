#ifndef UART_H
#define UART_H

#include "main.h"
#include <stdint.h>

typedef struct{
    UART_HandleTypeDef *huart;
    uint32_t timeout_ms;
}uart_t;

void uart_init(uart_t *uart, UART_HandleTypeDef *huart, uint32_t timeout_ms);
HAL_StatusTypeDef uart_send_byte(uart_t *uart,uint8_t byte);
HAL_StatusTypeDef uart_send_string(uart_t *uart,const char *text);
HAL_StatusTypeDef uart_receive_byte(uart_t *uart, uint8_t *received_byte);


#endif /* UART_H */