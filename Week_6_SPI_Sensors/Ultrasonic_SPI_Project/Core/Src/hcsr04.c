#include "hcsr04.h"

static uint32_t micros_now(TIM_HandleTypeDef *htim)
{
    return __HAL_TIM_GET_COUNTER(htim);
}

static void delay_us(TIM_HandleTypeDef *htim, uint32_t us)
{
    uint32_t start = micros_now(htim);
    while ((micros_now(htim) - start) < us) {
        /* spin */
    }
}

void hcsr04_init(hcsr04_t *dev,
                 GPIO_TypeDef *trig_port, uint16_t trig_pin,
                 GPIO_TypeDef *echo_port, uint16_t echo_pin,
                 TIM_HandleTypeDef *htim_us,
                 uint32_t timeout_us)
{
    dev->trig_port = trig_port;
    dev->trig_pin = trig_pin;
    dev->echo_port = echo_port;
    dev->echo_pin = echo_pin;
    dev->htim_us = htim_us;
    dev->timeout_us = timeout_us;

    HAL_GPIO_WritePin(dev->trig_port, dev->trig_pin, GPIO_PIN_RESET);
    HAL_TIM_Base_Start(dev->htim_us);
}

hcsr04_status_t hcsr04_read_cm(hcsr04_t *dev, float *distance_cm)
{
    uint32_t t0;
    uint32_t echo_start;
    uint32_t echo_end;
    uint32_t pulse_us;

    if (dev == NULL || distance_cm == NULL || dev->htim_us == NULL) {
        return HCSR04_ERROR;
    }

    /* 10 us trigger pulse (datasheet minimum) */
    HAL_GPIO_WritePin(dev->trig_port, dev->trig_pin, GPIO_PIN_SET);
    delay_us(dev->htim_us, 10);
    HAL_GPIO_WritePin(dev->trig_port, dev->trig_pin, GPIO_PIN_RESET);

    /* Wait for ECHO to go high */
    t0 = micros_now(dev->htim_us);
    while (HAL_GPIO_ReadPin(dev->echo_port, dev->echo_pin) == GPIO_PIN_RESET) {
        if ((micros_now(dev->htim_us) - t0) > dev->timeout_us) {
            return HCSR04_TIMEOUT;
        }
    }
    echo_start = micros_now(dev->htim_us);

    /* Wait for ECHO to go low */
    while (HAL_GPIO_ReadPin(dev->echo_port, dev->echo_pin) == GPIO_PIN_SET) {
        if ((micros_now(dev->htim_us) - echo_start) > dev->timeout_us) {
            return HCSR04_TIMEOUT;
        }
    }
    echo_end = micros_now(dev->htim_us);

    pulse_us = echo_end - echo_start;
    /* speed of sound ~343 m/s → cm = us / 58 */
    *distance_cm = (float)pulse_us / 58.0f;
    return HCSR04_OK;
}

hcsr04_status_t hcsr04_read_mm(hcsr04_t *dev, uint16_t *distance_mm)
{
    float cm;
    hcsr04_status_t st = hcsr04_read_cm(dev, &cm);
    if (st != HCSR04_OK) {
        return st;
    }
    if (cm < 0.0f) {
        cm = 0.0f;
    }
    *distance_mm = (uint16_t)(cm * 10.0f);
    return HCSR04_OK;
}
