#ifndef BUTTON_H
#define BUTTON_H

#include "gpio.h"
#include <stdbool.h>

typedef struct{
    gpio_pin_t pin;
    bool active_low;
    gpio_state_t last_state;
    bool pressed_event;
}button_t;

void button_init(button_t *button, gpio_pull_t pull);
void button_update(button_t *button);
bool button_was_pressed(button_t *button);
bool button_is_pressed(const button_t *button);

#endif /* BUTTON_H */