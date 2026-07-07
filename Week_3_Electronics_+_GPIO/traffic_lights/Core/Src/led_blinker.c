/*
 * led_blinker.c
 *
 *  Created on: Jul 6, 2026
 *      Author: nishp
 */

#include "led_blinker.h"
#include "main.h"

void led_blinker_init(led_blinker *led, char port, uint8_t pin, uint32_t interval_ms){
	led->port = port;
	led->pin = pin;
	led->interval_ms = interval_ms;
	led->last_change = HAL_GetTick();
	led->state = GPIO_LOW;

	gpio_write(port, pin, led->state);
}
void led_blinker_update(led_blinker *led, uint32_t now){
	if (now - led->last_change >= led->interval_ms ){
		if (led->state == GPIO_LOW){
			led->state = GPIO_HIGH;
		}
		else{
			led->state = GPIO_LOW;
		}
		gpio_write(led->port, led->pin, led->state);
		led->last_change = now;

	}
}

