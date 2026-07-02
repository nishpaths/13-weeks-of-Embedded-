/*
 * gpio.c
 *
 *  Created on: Jul 1, 2026
 *      Author: nishp
 */
#include "gpio.h" // all of my methods
#include "main.h" // all of the pins are defined here
#include "stm32f4xx_hal_gpio.h" // contains the gpio methods like writing and reading and toggling a pin.

void gpio_toggle(char port, uint8_t pin){
	if(port == 'A' && pin == 5){
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	}
}
void gpio_write(char port, uint8_t pin, gpio_state_t state){
	if(port == 'A' && pin == 5){
		if(state == GPIO_HIGH){
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		}
		else if(state == GPIO_LOW){
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		}
	}
}
void gpio_output_init(char port, uint8_t pin)
{
    /*
     * Stub for now.
     *
     * PA5 / LD2 is currently initialized by CubeMX inside MX_GPIO_Init().
     * Later, this function should:
     * 1. Enable the GPIO port clock
     * 2. Configure the pin mode as output
     * 3. Set output type to push-pull
     * 4. Set speed
     * 5. Disable pull-up / pull-down
     */

    (void)port;
    (void)pin;
}



