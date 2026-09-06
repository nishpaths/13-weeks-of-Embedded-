#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Software debounce for a button sampled from an ISR edge flag.
 *
 * ISR only sets raw_edge. Call debounce_update() from main context
 * with HAL_GetTick() to confirm a stable press.
 */
typedef struct {
    uint32_t settle_ms;
    uint32_t edge_tick;
    bool pending;
    bool confirmed_press;
} debounce_t;

void debounce_init(debounce_t *db, uint32_t settle_ms);

/** Call from EXTI ISR (keep tiny). */
void debounce_on_isr_edge(debounce_t *db, uint32_t now_ms);

/** Call from main loop. */
void debounce_update(debounce_t *db, uint32_t now_ms, bool pin_is_active);

/** Returns true once per confirmed press; clears the latch. */
bool debounce_take_press(debounce_t *db);

#endif /* DEBOUNCE_H */
