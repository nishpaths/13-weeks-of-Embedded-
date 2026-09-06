#ifndef APP_FREERTOS_H
#define APP_FREERTOS_H

#include "main.h"

/**
 * Call once after MX_*_Init and before osKernelStart / vTaskStartScheduler.
 * Creates queues, mutex, and tasks.
 */
void app_freertos_start(void);

#endif /* APP_FREERTOS_H */
