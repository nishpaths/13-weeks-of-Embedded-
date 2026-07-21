#include "pwm.h"

void pwm_init(pwm_t *pwm, TIM_HandleTypeDef *htim, uint32_t channel){
    pwm->htim = htim;
    pwm->channel = channel;
}
void pwm_start(const pwm_t *pwm){
    HAL_TIM_PWM_Start(pwm->htim, pwm->channel);
}
void pwm_set_duty(const pwm_t *pwm, uint8_t percentage){
    if (percentage > 100){
      percentage = 100;
    }
    uint32_t period = __HAL_TIM_GET_AUTORELOAD(pwm->htim);
    uint32_t pulse = (period + 1) * percentage / 100;
    __HAL_TIM_SET_COMPARE(pwm->htim, pwm->channel, pulse);
}