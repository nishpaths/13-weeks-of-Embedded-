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

typedef enum {
    GPIO_PULL_NONE,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN
} gpio_pull_t;


void gpio_output_init(char port, uint8_t pin);
void gpio_input_init(char port, uint8_t pin, gpio_pull_t pull);
gpio_state_t gpio_read(char port, uint8_t pin);

void gpio_toggle(char port, uint8_t pin);
void gpio_write(char port, uint8_t pin, gpio_state_t state);


#endif /* INC_GPIO_H_ */
