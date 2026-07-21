#include "main.h"

#ifndef PWM_H
#define PWM_H

typedef struct{
    TIM_HandleTypeDef *htim;
    uint32_t channel;
}pwm_t;

void pwm_init(pwm_t *pwm, TIM_HandleTypeDef *htim, uint32_t channel);
void pwm_start(const pwm_t *pwm);
void pwm_set_duty(const pwm_t *pwm, uint8_t percentage);

#endif
