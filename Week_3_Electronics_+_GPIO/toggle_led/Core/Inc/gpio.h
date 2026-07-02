/*
 * gpio.h
 *
 *  Created on: Jul 1, 2026
 *      Author: nishp
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include <stdint.h>

typedef enum{
	GPIO_LOW = 0,
	GPIO_HIGH = 1
}gpio_state_t;

void gpio_toggle(char port, uint8_t pin);
void gpio_write(char port, uint8_t pin, gpio_state_t state);


#endif /* INC_GPIO_H_ */
