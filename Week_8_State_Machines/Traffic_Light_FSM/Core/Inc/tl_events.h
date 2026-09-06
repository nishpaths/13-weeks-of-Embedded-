#ifndef TL_EVENTS_H
#define TL_EVENTS_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    TL_EVT_NONE = 0,
    TL_EVT_TICK_EXPIRED,
    TL_EVT_PED_REQUEST
} tl_event_t;

typedef struct {
    volatile bool ped_raw;
    bool ped_latched;
} tl_event_queue_t;

void tl_events_init(tl_event_queue_t *q);
void tl_events_on_ped_isr(tl_event_queue_t *q);
void tl_events_poll_ped(tl_event_queue_t *q, bool ped_active_now, uint32_t now_ms);
tl_event_t tl_events_next(tl_event_queue_t *q, bool timer_expired);

#endif /* TL_EVENTS_H */
