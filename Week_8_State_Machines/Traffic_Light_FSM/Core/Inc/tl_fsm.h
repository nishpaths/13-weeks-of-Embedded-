#ifndef TL_FSM_H
#define TL_FSM_H

#include "tl_gpio.h"
#include "tl_events.h"
#include <stdint.h>

typedef enum {
    TL_GREEN = 0,
    TL_YELLOW,
    TL_RED,
    TL_RED_PED,
    TL_ALL_RED
} tl_state_t;

typedef struct {
    uint32_t green_ms;
    uint32_t yellow_ms;
    uint32_t red_ms;
    uint32_t ped_ms;
    uint32_t all_red_ms;
} tl_timing_t;

typedef struct {
    tl_state_t state;
    tl_lamps_t *lamps;
    tl_timing_t timing;
    tl_event_queue_t events;
    uint32_t state_entered_ms;
    bool ped_pending;
} tl_fsm_t;

void tl_fsm_init(tl_fsm_t *fsm, tl_lamps_t *lamps, const tl_timing_t *timing);
void tl_fsm_on_ped_isr(tl_fsm_t *fsm);
void tl_fsm_update(tl_fsm_t *fsm, uint32_t now_ms, bool ped_active_now);
tl_state_t tl_fsm_state(const tl_fsm_t *fsm);
const char *tl_fsm_state_name(tl_state_t s);

#endif /* TL_FSM_H */
