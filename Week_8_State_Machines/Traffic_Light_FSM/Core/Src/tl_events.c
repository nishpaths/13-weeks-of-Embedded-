#include "tl_events.h"

void tl_events_init(tl_event_queue_t *q)
{
    q->ped_raw = false;
    q->ped_latched = false;
}

void tl_events_on_ped_isr(tl_event_queue_t *q)
{
    q->ped_raw = true;
}

void tl_events_poll_ped(tl_event_queue_t *q, bool ped_active_now, uint32_t now_ms)
{
    static uint32_t edge_ms;
    static bool pending;

    (void)now_ms;

    if (q->ped_raw) {
        q->ped_raw = false;
        pending = true;
        edge_ms = now_ms;
    }

    if (pending && (now_ms - edge_ms) >= 30U) {
        pending = false;
        if (ped_active_now) {
            q->ped_latched = true;
        }
    }
}

tl_event_t tl_events_next(tl_event_queue_t *q, bool timer_expired)
{
    if (q->ped_latched) {
        q->ped_latched = false;
        return TL_EVT_PED_REQUEST;
    }
    if (timer_expired) {
        return TL_EVT_TICK_EXPIRED;
    }
    return TL_EVT_NONE;
}
