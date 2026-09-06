#ifndef APP_UART_H
#define APP_UART_H

#include "main.h"
#include <stdint.h>

typedef struct {
    UART_HandleTypeDef *huart;
    uint32_t timeout_ms;
} app_uart_t;

void app_uart_init(app_uart_t *u, UART_HandleTypeDef *huart, uint32_t timeout_ms);
void app_uart_print(app_uart_t *u, const char *text);
void app_uart_printf(app_uart_t *u, const char *fmt, ...);

#endif /* APP_UART_H */
