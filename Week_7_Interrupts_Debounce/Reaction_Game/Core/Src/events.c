#include "events.h"

void events_init(event_flags_t *ev)
{
    ev->button_edge = false;
    ev->timer_fired = false;
}

void events_set_button(event_flags_t *ev)
{
    ev->button_edge = true;
}

void events_set_timer(event_flags_t *ev)
{
    ev->timer_fired = true;
}

bool events_take_button(event_flags_t *ev)
{
    if (ev->button_edge) {
        ev->button_edge = false;
        return true;
    }
    return false;
}

bool events_take_timer(event_flags_t *ev)
{
    if (ev->timer_fired) {
        ev->timer_fired = false;
        return true;
    }
    return false;
}
