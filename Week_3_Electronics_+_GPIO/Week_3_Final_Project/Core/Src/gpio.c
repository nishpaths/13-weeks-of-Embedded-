#include "gpio.h" // all of my methods
#include "main.h" // all of the pins are defined here
#include "stm32f4xx_hal_gpio.h" // contains the gpio methods like writing and reading and toggling a pin.

void gpio_toggle(const gpio_pin_t *pin){
	GPIO_TypeDef *gpio_port;

	if (pin->port == 'A') {
		gpio_port = GPIOA;
	}
	else if (pin->port == 'B') {
		gpio_port = GPIOB;
	}
	else if (pin->port == 'C') {
		gpio_port = GPIOC;
	}
	else {
		return;
	}

	uint16_t gpio_pin = (1 << pin->pin);

	HAL_GPIO_TogglePin(gpio_port, gpio_pin);
}
void gpio_write(const gpio_pin_t *pin, gpio_state_t state){
	GPIO_TypeDef *gpio_port;

	if (pin->port == 'A') {
		gpio_port = GPIOA;
	}
	else if (pin->port == 'B') {
		gpio_port = GPIOB;
	}
	else {
		return;
	}

	uint16_t gpio_pin = (1 << pin->pin);

	if (state == GPIO_HIGH) {
		HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_RESET);
	}
}
void gpio_output_init(const gpio_pin_t *pin)
{

	GPIO_TypeDef *gpio_port;

	if(pin->port == 'A'){
		gpio_port = GPIOA;
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(pin->port == 'B'){
		gpio_port = GPIOB;
		__HAL_RCC_GPIOB_CLK_ENABLE();

	}
	else if(pin->port == 'C'){
		gpio_port = GPIOC;
		__HAL_RCC_GPIOC_CLK_ENABLE();

	}
	else{
		return;
	}
	//configure the pin
	uint16_t pin_mask = (uint16_t)(1 << pin->pin);

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = pin_mask;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(gpio_port, &GPIO_InitStruct);
}

void gpio_input_init(const gpio_pin_t *pin, gpio_pull_t pull){
		GPIO_TypeDef *gpio_port;


		if(pin->port == 'A'){
			gpio_port = GPIOA;
			__HAL_RCC_GPIOA_CLK_ENABLE();
		}
		else if(pin->port == 'B'){
			gpio_port = GPIOB;
			__HAL_RCC_GPIOB_CLK_ENABLE();

		}
		else if(pin->port == 'C'){
			gpio_port = GPIOC;
			__HAL_RCC_GPIOC_CLK_ENABLE();

		}
		else{
			return;
		}
		//configure the pin
		uint16_t pin_mask = (uint16_t)(1U << pin->pin);

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
gpio_state_t gpio_read(const gpio_pin_t *pin){
	    GPIO_TypeDef *gpio_port;

	    if (pin->port == 'A')
	    {
	        gpio_port = GPIOA;
	    }
	    else if (pin->port == 'B')
	    {
	        gpio_port = GPIOB;
	    }
	    else if (pin->port == 'C')
	    {
	        gpio_port = GPIOC;
	    }
	    else
	    {
	        return GPIO_LOW; // temporary bad-port fallback
	    }

	    GPIO_PinState state = HAL_GPIO_ReadPin(gpio_port, (1U << pin->pin));

	    if (state == GPIO_PIN_RESET)
	    {
	        return GPIO_LOW;
	    }
	    else
	    {
	        return GPIO_HIGH;
	    }
}