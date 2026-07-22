#include "uart.h"
#include <string.h>

void uart_init(uart_t *uart, UART_HandleTypeDef *huart, uint32_t timeout_ms){
    uart->huart = huart;
    uart->timeout_ms = timeout_ms;
}
HAL_StatusTypeDef uart_send_byte(uart_t *uart,uint8_t byte){
    if (uart == NULL || uart->huart == NULL) {
        return HAL_ERROR; // Return an error if the UART handle is not initialized
    }
    return HAL_UART_Transmit(uart->huart, &byte, 1, uart->timeout_ms);
}
HAL_StatusTypeDef uart_send_string(uart_t *uart,const char *text){
    if (uart == NULL || uart->huart == NULL) {
        return HAL_ERROR; // Return an error if the UART handle is not initialized
    }
    return HAL_UART_Transmit(uart->huart, (uint8_t*)text, strlen(text), uart->timeout_ms);
}
HAL_StatusTypeDef uart_receive_byte(uart_t *uart,uint8_t *received_byte){
    if (uart == NULL || uart->huart == NULL) {
        return HAL_ERROR; // Return an error if the UART handle is not initialized
    }
    return HAL_UART_Receive(uart->huart, received_byte, 1, uart->timeout_ms);
}