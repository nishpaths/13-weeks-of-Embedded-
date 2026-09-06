#ifndef HCSR04_H
#define HCSR04_H

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * HC-SR04 ultrasonic ranging module.
 *
 * Datasheet timing (typical):
 *   - TRIG: >= 10 us high pulse
 *   - ECHO high time (us) / 58 ~= distance in cm
 *   - Max range ~400 cm; timeout if no echo
 *
 * Requires a free-running timer ticking at 1 MHz (1 tick = 1 us).
 */
typedef struct {
    GPIO_TypeDef *trig_port;
    uint16_t trig_pin;
    GPIO_TypeDef *echo_port;
    uint16_t echo_pin;
    TIM_HandleTypeDef *htim_us; /* 1 MHz free-running counter */
    uint32_t timeout_us;
} hcsr04_t;

typedef enum {
    HCSR04_OK = 0,
    HCSR04_TIMEOUT,
    HCSR04_ERROR
} hcsr04_status_t;

void hcsr04_init(hcsr04_t *dev,
                 GPIO_TypeDef *trig_port, uint16_t trig_pin,
                 GPIO_TypeDef *echo_port, uint16_t echo_pin,
                 TIM_HandleTypeDef *htim_us,
                 uint32_t timeout_us);

hcsr04_status_t hcsr04_read_cm(hcsr04_t *dev, float *distance_cm);
hcsr04_status_t hcsr04_read_mm(hcsr04_t *dev, uint16_t *distance_mm);

#endif /* HCSR04_H */
