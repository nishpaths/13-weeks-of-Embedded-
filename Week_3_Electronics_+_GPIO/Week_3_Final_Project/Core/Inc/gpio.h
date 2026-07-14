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

typedef struct{
    char port;
    uint8_t pin;
}gpio_pin_t;

void gpio_output_init(const gpio_pin_t *pin);
void gpio_input_init(const gpio_pin_t *pin, gpio_pull_t pull);
gpio_state_t gpio_read(const gpio_pin_t *pin);
void gpio_toggle(const gpio_pin_t *pin);
void gpio_write(const gpio_pin_t *pin, gpio_state_t state);



#endif /* INC_GPIO_H_ */