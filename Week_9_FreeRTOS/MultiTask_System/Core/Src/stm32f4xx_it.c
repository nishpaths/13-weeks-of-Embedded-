/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines — Week 9 (FreeRTOS)
  ******************************************************************************
  */
/* USER CODE END Header */
#include "main.h"
#include "stm32f4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"

extern UART_HandleTypeDef huart2;

extern void xPortSysTickHandler(void);

void NMI_Handler(void)
{
  while (1) {
  }
}

void HardFault_Handler(void)
{
  while (1) {
  }
}

void MemManage_Handler(void)
{
  while (1) {
  }
}

void BusFault_Handler(void)
{
  while (1) {
  }
}

void UsageFault_Handler(void)
{
  while (1) {
  }
}

/* SVC_Handler / PendSV_Handler provided by FreeRTOS port via FreeRTOSConfig.h */

void DebugMon_Handler(void)
{
}

void SysTick_Handler(void)
{
  HAL_IncTick();
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    xPortSysTickHandler();
  }
}

void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart2);
}

void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
