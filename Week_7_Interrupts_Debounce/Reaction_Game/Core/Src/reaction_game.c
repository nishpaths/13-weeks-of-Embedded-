#include "reaction_game.h"

static void led_on(reaction_game_t *g)
{
    HAL_GPIO_WritePin(g->led_port, g->led_pin, GPIO_PIN_SET);
}

static void led_off(reaction_game_t *g)
{
    HAL_GPIO_WritePin(g->led_port, g->led_pin, GPIO_PIN_RESET);
}

static bool button_active(const reaction_game_t *g)
{
    GPIO_PinState s = HAL_GPIO_ReadPin(g->btn_port, g->btn_pin);
    if (g->btn_active_low) {
        return s == GPIO_PIN_RESET;
    }
    return s == GPIO_PIN_SET;
}

static uint32_t next_delay_ms(reaction_game_t *g)
{
    /* xorshift-ish using SysTick + seed — good enough for game delay */
    g->seed ^= g->seed << 13;
    g->seed ^= g->seed >> 17;
    g->seed ^= g->seed << 5;
    return 1000U + (g->seed % 3001U); /* 1000..4000 ms */
}

static void arm_random_timer(reaction_game_t *g)
{
    uint32_t delay_ms = next_delay_ms(g);
    uint32_t clk = HAL_RCC_GetPCLK1Freq();
    /* TIM3 on APB1; if APB1 prescaler != 1, timer clock is 2x PCLK1 on F4 */
    uint32_t tim_clk = clk;
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1) {
        tim_clk = clk * 2U;
    }

    /* 10 kHz timer tick → ARR = delay_ms * 10 - 1 */
    uint32_t psc = (tim_clk / 10000U) - 1U;
    uint32_t arr = (delay_ms * 10U) - 1U;

    __HAL_TIM_DISABLE(g->htim);
    __HAL_TIM_SET_PRESCALER(g->htim, psc);
    __HAL_TIM_SET_AUTORELOAD(g->htim, arr);
    __HAL_TIM_SET_COUNTER(g->htim, 0);
    g->htim->Instance->EGR = TIM_EGR_UG; /* latch PSC/ARR */
    __HAL_TIM_CLEAR_FLAG(g->htim, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(g->htim);
}

void reaction_game_init(reaction_game_t *g,
                        TIM_HandleTypeDef *htim,
                        GPIO_TypeDef *led_port, uint16_t led_pin,
                        GPIO_TypeDef *btn_port, uint16_t btn_pin,
                        bool btn_active_low)
{
    g->state = RG_IDLE;
    g->htim = htim;
    g->led_port = led_port;
    g->led_pin = led_pin;
    g->btn_port = btn_port;
    g->btn_pin = btn_pin;
    g->btn_active_low = btn_active_low;
    g->go_tick_ms = 0;
    g->reaction_ms = 0;
    g->result_until_ms = 0;
    g->seed = 0xA5A5u ^ HAL_GetTick();

    debounce_init(&g->debounce, 30);
    events_init(&g->events);
    led_off(g);
}

void reaction_game_on_exti(reaction_game_t *g, uint16_t pin)
{
    if (pin == g->btn_pin) {
        events_set_button(&g->events);
        debounce_on_isr_edge(&g->debounce, HAL_GetTick());
    }
}

void reaction_game_on_timer(reaction_game_t *g, TIM_HandleTypeDef *htim)
{
    if (htim == g->htim) {
        HAL_TIM_Base_Stop_IT(g->htim);
        events_set_timer(&g->events);
    }
}

bool reaction_game_update(reaction_game_t *g, uint32_t now_ms)
{
    bool new_result = false;

    if (events_take_button(&g->events)) {
        /* raw edge already recorded for debounce */
    }

    debounce_update(&g->debounce, now_ms, button_active(g));
    bool press = debounce_take_press(&g->debounce);
    bool timer = events_take_timer(&g->events);

    switch (g->state) {
    case RG_IDLE:
        led_off(g);
        arm_random_timer(g);
        g->state = RG_WAIT_RANDOM;
        break;

    case RG_WAIT_RANDOM:
        if (press) {
            HAL_TIM_Base_Stop_IT(g->htim);
            g->state = RG_FALSE_START;
            g->result_until_ms = now_ms + 1000U;
            led_on(g);
        } else if (timer) {
            led_on(g);
            g->go_tick_ms = now_ms;
            g->state = RG_WAIT_PRESS;
        }
        break;

    case RG_WAIT_PRESS:
        if (press) {
            g->reaction_ms = now_ms - g->go_tick_ms;
            g->state = RG_SHOW_RESULT;
            g->result_until_ms = now_ms + 1500U;
            new_result = true;
        }
        break;

    case RG_SHOW_RESULT:
    case RG_FALSE_START:
        if (now_ms >= g->result_until_ms) {
            led_off(g);
            g->state = RG_IDLE;
        } else if ((now_ms / 100U) % 2U == 0U) {
            led_on(g);
        } else {
            led_off(g);
        }
        break;
    }

    return new_result;
}

uint32_t reaction_game_last_reaction_ms(const reaction_game_t *g)
{
    return g->reaction_ms;
}

reaction_state_t reaction_game_state(const reaction_game_t *g)
{
    return g->state;
}
