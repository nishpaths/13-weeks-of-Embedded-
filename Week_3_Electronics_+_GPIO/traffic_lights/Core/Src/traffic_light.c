/*
 * traffic_light.c
 *
 *  Created on: Jul 6, 2026
 *      Author: nishp
 */
#include "traffic_light.h"

#define GREEN_DURATION_MS 5000
#define YELLOW_DURATION_MS 2000
#define RED_DURATION_MS 5000

typedef enum{
	TRAFFIC_GREEN,
	TRAFFIC_YELLOW,
	TRAFFIC_RED
} traffic_state;

static traffic_state current_state = TRAFFIC_GREEN;
static uint32_t last_change = 0;

void traffic_light_init(void){
	current_state = TRAFFIC_GREEN;
	last_change = HAL_GetTick();
	set_green();
}

void set_green(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
}
void set_yellow(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
}
void set_red(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
}

void traffic_light_update(uint32_t now){
	if (current_state == TRAFFIC_GREEN){
		if(now - last_change >= GREEN_DURATION_MS){
			current_state = TRAFFIC_YELLOW;
			set_yellow();
			last_change = now;
		}
	}
	else if(current_state == TRAFFIC_YELLOW){
		if(now - last_change >= YELLOW_DURATION_MS){
			current_state = TRAFFIC_RED;
			set_red();
			last_change = now;
		}
	}
	else if(current_state == TRAFFIC_RED){
		if(now - last_change >= RED_DURATION_MS){
			current_state = TRAFFIC_GREEN;
			set_green();
			last_change = now;
		}
	}

}

