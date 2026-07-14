#include "button.h"


/*typedef struct{
    gpio_pin_t pin;
    bool active_low;
    gpio_state_t last_state;
    bool pressed_event;
}button_t;*/

void button_init(button_t *button, gpio_pull_t pull){
    gpio_input_init(&button->pin, pull);

   button->pressed_event = false;
   button-> last_state = gpio_read(&button->pin);
}
void button_update(button_t *button){
    gpio_state_t current_state = gpio_read(&button->pin);

    if (current_state == GPIO_LOW && button->last_state == GPIO_HIGH) {
        button->pressed_event = true;
    } else {
        button->pressed_event = false;
    }

    button->last_state = current_state;
}
bool button_was_pressed(button_t *button){
    return button->pressed_event;
}
bool button_is_pressed(const button_t *button){
    if (button->active_low) {
        return button->last_state == GPIO_LOW;
    } else {
        return button->last_state == GPIO_HIGH;
    }
}
