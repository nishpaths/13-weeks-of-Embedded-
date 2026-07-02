/*
 * delay.c
 *
 *  Created on: Jul 1, 2026
 *      Author: nishp
 */
#include "delay.h"
#include "stm32f4xx_hal.h"

void delay_ms(uint32_t ms){
	HAL_Delay(ms);
}



