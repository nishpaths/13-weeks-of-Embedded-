/*
 * gpio.c
 *
 *  Created on: Jul 6, 2026
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
	GPIO_TypeDef *gpio_port;

	if (port == 'A') {
		gpio_port = GPIOA;
	}
	else if (port == 'B') {
		gpio_port = GPIOB;
	}
	else {
		return;
	}

	uint16_t gpio_pin = (1 << pin);

	if (state == GPIO_HIGH) {
		HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_RESET);
	}
}
void gpio_output_init(char port, uint8_t pin)
{

	GPIO_TypeDef *gpio_port;

	if(port == 'A'){
		gpio_port = GPIOA;
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(port == 'B'){
		gpio_port = GPIOB;
		__HAL_RCC_GPIOB_CLK_ENABLE();

	}
	else if(port == 'C'){
		gpio_port = GPIOC;
		__HAL_RCC_GPIOC_CLK_ENABLE();

	}
	else{
		return;
	}
	//configure the pin
	uint16_t pin_mask = (uint16_t)(1 << pin);

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = pin_mask;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(gpio_port, &GPIO_InitStruct);
}

void gpio_input_init(char port, uint8_t pin, gpio_pull_t pull){
		GPIO_TypeDef *gpio_port;


		if(port == 'A'){
			gpio_port = GPIOA;
			__HAL_RCC_GPIOA_CLK_ENABLE();
		}
		else if(port == 'B'){
			gpio_port = GPIOB;
			__HAL_RCC_GPIOB_CLK_ENABLE();

		}
		else if(port == 'C'){
			gpio_port = GPIOC;
			__HAL_RCC_GPIOC_CLK_ENABLE();

		}
		else{
			return;
		}
		//configure the pin
		uint16_t pin_mask = (uint16_t)(1U << pin);

		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = pin_mask;

		if (pull == GPIO_PULL_NONE){
			GPIO_InitStruct.Pull = GPIO_NOPULL;
		}
		else if(pull == GPIO_PULL_UP){
			GPIO_InitStruct.Pull = GPIO_PULLUP;
		}
		else{
			GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		}


		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		//GPIO_InitStruct.Pull = pull;

		HAL_GPIO_Init(gpio_port, &GPIO_InitStruct);

}
gpio_state_t gpio_read(char port, uint8_t pin){
	    GPIO_TypeDef *gpio_port;

	    if (port == 'A')
	    {
	        gpio_port = GPIOA;
	    }
	    else if (port == 'B')
	    {
	        gpio_port = GPIOB;
	    }
	    else if (port == 'C')
	    {
	        gpio_port = GPIOC;
	    }
	    else
	    {
	        return GPIO_LOW; // temporary bad-port fallback
	    }

	    GPIO_PinState state = HAL_GPIO_ReadPin(gpio_port, (1U << pin));

	    if (state == GPIO_PIN_RESET)
	    {
	        return GPIO_LOW;
	    }
	    else
	    {
	        return GPIO_HIGH;
	    }
}

