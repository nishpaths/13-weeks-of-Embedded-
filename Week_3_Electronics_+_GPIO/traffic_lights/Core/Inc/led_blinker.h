/*
 * led_blinker.h
 *
 *  Created on: Jul 6, 2026
 *      Author: nishp
 */

#ifndef INC_LED_BLINKER_H_
#define INC_LED_BLINKER_H_

#include "gpio.h"

typedef struct{
	char port;
	uint8_t pin;
	uint32_t interval_ms;
	uint32_t last_change;
	gpio_state_t state;
}led_blinker;

void led_blinker_init(led_blinker *led, char port, uint8_t pin, uint32_t interval_ms);
void led_blinker_update(led_blinker *led, uint32_t now);


#endif /* INC_LED_BLINKER_H_ */
