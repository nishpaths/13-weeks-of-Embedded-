#include "led.h"

void led_init(led_t *led, gpio_pin_t pin, bool active_high){
    led->pin = pin;
    led->active_high = active_high;
    gpio_output_init(&led->pin);
}

void led_on(const led_t *led){
    if (led->active_high == true){
        gpio_write(&led->pin, GPIO_HIGH);
    }
    else{
        gpio_write(&led->pin, GPIO_LOW);
    }
}
void led_off(const led_t *led){
    if (led->active_high == true){
        gpio_write(&led->pin, GPIO_LOW);
    }
    else{
        gpio_write(&led->pin, GPIO_HIGH);
    }
}
void led_toggle(const led_t *led){
    gpio_toggle(&led->pin);
}