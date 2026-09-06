#include "app_uart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void app_uart_init(app_uart_t *u, UART_HandleTypeDef *huart, uint32_t timeout_ms)
{
    u->huart = huart;
    u->timeout_ms = timeout_ms;
}

void app_uart_print(app_uart_t *u, const char *text)
{
    if (u == NULL || u->huart == NULL || text == NULL) {
        return;
    }
    HAL_UART_Transmit(u->huart, (uint8_t *)text, (uint16_t)strlen(text), u->timeout_ms);
}

void app_uart_printf(app_uart_t *u, const char *fmt, ...)
{
    char buf[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    app_uart_print(u, buf);
}
