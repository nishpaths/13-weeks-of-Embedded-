/**
 * Week 9 FreeRTOS multi-task application (native FreeRTOS API).
 */
#include "app_freertos.h"
#include "sensor_sample.h"
#include "hcsr04.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;

#ifndef USE_REAL_SONAR
#define USE_REAL_SONAR 1
#endif

static QueueHandle_t qDisplay;
static QueueHandle_t qLog;
static SemaphoreHandle_t mtxUart;

static hcsr04_t sonar;
static float stub_cm = 30.0f;

static void uart_locked_print(const char *s)
{
  xSemaphoreTake(mtxUart, portMAX_DELAY);
  HAL_UART_Transmit(&huart2, (uint8_t *)s, (uint16_t)strlen(s), 100);
  xSemaphoreGive(mtxUart);
}

static void SensorTask(void *arg)
{
  (void)arg;
  sensor_sample_t sample;

  for (;;) {
    sample.tick_ms = HAL_GetTick();
#if USE_REAL_SONAR
    {
      float cm = 0.0f;
      sample.valid = (hcsr04_read_cm(&sonar, &cm) == HCSR04_OK);
      sample.distance_cm = cm;
    }
#else
    stub_cm += 1.5f;
    if (stub_cm > 100.0f) {
      stub_cm = 20.0f;
    }
    sample.distance_cm = stub_cm;
    sample.valid = true;
#endif
    (void)xQueueSend(qDisplay, &sample, 0);
    (void)xQueueSend(qLog, &sample, 0);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

static void DisplayTask(void *arg)
{
  (void)arg;
  sensor_sample_t sample;
  char line[48];

  for (;;) {
    if (xQueueReceive(qDisplay, &sample, portMAX_DELAY) == pdTRUE) {
      if (sample.valid) {
        snprintf(line, sizeof(line), "[OLED] %.1f cm\r\n", (double)sample.distance_cm);
      } else {
        snprintf(line, sizeof(line), "[OLED] --\r\n");
      }
      uart_locked_print(line);
    }
  }
}

static void LogTask(void *arg)
{
  (void)arg;
  sensor_sample_t sample;
  char line[64];

  for (;;) {
    if (xQueueReceive(qLog, &sample, portMAX_DELAY) == pdTRUE) {
      if (sample.valid) {
        snprintf(line, sizeof(line), "[LOG] dist=%.1f cm t=%lu\r\n",
                 (double)sample.distance_cm, (unsigned long)sample.tick_ms);
      } else {
        snprintf(line, sizeof(line), "[LOG] timeout t=%lu\r\n",
                 (unsigned long)sample.tick_ms);
      }
      uart_locked_print(line);
    }
  }
}

static void HeartbeatTask(void *arg)
{
  (void)arg;
  uint32_t beats = 0;
  for (;;) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    beats++;
    if ((beats % 4U) == 0U) {
      uart_locked_print("[HB] alive\r\n");
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void app_freertos_start(void)
{
#if USE_REAL_SONAR
  hcsr04_init(&sonar, GPIOB, GPIO_PIN_0, GPIOB, GPIO_PIN_1, &htim2, 30000);
#endif

  qDisplay = xQueueCreate(8, sizeof(sensor_sample_t));
  qLog = xQueueCreate(8, sizeof(sensor_sample_t));
  mtxUart = xSemaphoreCreateMutex();

  xTaskCreate(SensorTask, "sensor", 512, NULL, tskIDLE_PRIORITY + 2, NULL);
  xTaskCreate(DisplayTask, "display", 512, NULL, tskIDLE_PRIORITY + 2, NULL);
  xTaskCreate(LogTask, "log", 512, NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(HeartbeatTask, "heartbeat", 256, NULL, tskIDLE_PRIORITY + 1, NULL);

  vTaskStartScheduler();
}
