#ifndef REACTION_GAME_H
#define REACTION_GAME_H

#include "debounce.h"
#include "events.h"
#include "main.h"
#include <stdint.h>

typedef enum {
    RG_IDLE = 0,
    RG_WAIT_RANDOM,
    RG_WAIT_PRESS,
    RG_SHOW_RESULT,
    RG_FALSE_START
} reaction_state_t;

typedef struct {
    reaction_state_t state;
    TIM_HandleTypeDef *htim;
    GPIO_TypeDef *led_port;
    uint16_t led_pin;
    GPIO_TypeDef *btn_port;
    uint16_t btn_pin;
    bool btn_active_low;
    debounce_t debounce;
    event_flags_t events;
    uint32_t go_tick_ms;
    uint32_t reaction_ms;
    uint32_t result_until_ms;
    uint32_t seed;
} reaction_game_t;

void reaction_game_init(reaction_game_t *g,
                        TIM_HandleTypeDef *htim,
                        GPIO_TypeDef *led_port, uint16_t led_pin,
                        GPIO_TypeDef *btn_port, uint16_t btn_pin,
                        bool btn_active_low);

/** Hook from HAL_GPIO_EXTI_Callback. */
void reaction_game_on_exti(reaction_game_t *g, uint16_t pin);

/** Hook from HAL_TIM_PeriodElapsedCallback. */
void reaction_game_on_timer(reaction_game_t *g, TIM_HandleTypeDef *htim);

/** Non-blocking update; returns true when a new result is ready to print. */
bool reaction_game_update(reaction_game_t *g, uint32_t now_ms);

uint32_t reaction_game_last_reaction_ms(const reaction_game_t *g);
reaction_state_t reaction_game_state(const reaction_game_t *g);

#endif /* REACTION_GAME_H */
