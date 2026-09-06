#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Tiny event flags set in ISRs, consumed in main.
 * Keep ISRs short: set a flag, return.
 */
typedef struct {
    volatile bool button_edge;
    volatile bool timer_fired;
} event_flags_t;

void events_init(event_flags_t *ev);
void events_set_button(event_flags_t *ev);
void events_set_timer(event_flags_t *ev);
bool events_take_button(event_flags_t *ev);
bool events_take_timer(event_flags_t *ev);

#endif /* EVENTS_H */
