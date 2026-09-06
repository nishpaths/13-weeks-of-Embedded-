#include "debounce.h"

void debounce_init(debounce_t *db, uint32_t settle_ms)
{
    db->settle_ms = settle_ms;
    db->edge_tick = 0;
    db->pending = false;
    db->confirmed_press = false;
}

void debounce_on_isr_edge(debounce_t *db, uint32_t now_ms)
{
    if (!db->pending) {
        db->pending = true;
        db->edge_tick = now_ms;
    }
}

void debounce_update(debounce_t *db, uint32_t now_ms, bool pin_is_active)
{
    if (!db->pending) {
        return;
    }

    if ((now_ms - db->edge_tick) < db->settle_ms) {
        return;
    }

    if (pin_is_active) {
        db->confirmed_press = true;
    }
    db->pending = false;
}

bool debounce_take_press(debounce_t *db)
{
    if (db->confirmed_press) {
        db->confirmed_press = false;
        return true;
    }
    return false;
}
