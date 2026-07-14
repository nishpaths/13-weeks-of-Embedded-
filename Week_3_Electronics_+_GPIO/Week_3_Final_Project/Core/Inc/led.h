#ifndef LED_H
#define LED_H

#include <stdbool.h>
#include "gpio.h"

typedef struct{
    gpio_pin_t pin;
    bool active_high;
}led_t;

void led_init(led_t *led, gpio_pin_t pin, bool active_high);
void led_on(const led_t *led);
void led_off(const led_t *led);
void led_toggle(const led_t *led);

#endif /* LED_H */