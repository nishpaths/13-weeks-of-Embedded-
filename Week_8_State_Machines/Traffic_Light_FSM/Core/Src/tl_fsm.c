#include "tl_fsm.h"

static uint32_t duration_for(const tl_fsm_t *fsm, tl_state_t s)
{
    switch (s) {
    case TL_GREEN:   return fsm->timing.green_ms;
    case TL_YELLOW:  return fsm->timing.yellow_ms;
    case TL_RED:     return fsm->timing.red_ms;
    case TL_RED_PED: return fsm->timing.ped_ms;
    case TL_ALL_RED: return fsm->timing.all_red_ms;
    default:         return 1000U;
    }
}

static void apply_outputs(tl_fsm_t *fsm)
{
    switch (fsm->state) {
    case TL_GREEN:   tl_lamps_set_green(fsm->lamps); break;
    case TL_YELLOW:  tl_lamps_set_yellow(fsm->lamps); break;
    case TL_RED:     tl_lamps_set_red(fsm->lamps); break;
    case TL_RED_PED: tl_lamps_set_red_ped(fsm->lamps); break;
    case TL_ALL_RED: tl_lamps_set_red(fsm->lamps); break;
    }
}

static void enter_state(tl_fsm_t *fsm, tl_state_t next, uint32_t now_ms)
{
    fsm->state = next;
    fsm->state_entered_ms = now_ms;
    apply_outputs(fsm);
}

void tl_fsm_init(tl_fsm_t *fsm, tl_lamps_t *lamps, const tl_timing_t *timing)
{
    fsm->lamps = lamps;
    fsm->timing = *timing;
    fsm->ped_pending = false;
    tl_events_init(&fsm->events);
    tl_lamps_init(lamps);
    enter_state(fsm, TL_GREEN, HAL_GetTick());
}

void tl_fsm_on_ped_isr(tl_fsm_t *fsm)
{
    tl_events_on_ped_isr(&fsm->events);
}

void tl_fsm_update(tl_fsm_t *fsm, uint32_t now_ms, bool ped_active_now)
{
    bool expired = (now_ms - fsm->state_entered_ms) >= duration_for(fsm, fsm->state);

    tl_events_poll_ped(&fsm->events, ped_active_now, now_ms);
    tl_event_t ev = tl_events_next(&fsm->events, expired);

    if (ev == TL_EVT_PED_REQUEST) {
        fsm->ped_pending = true;
    }

    if (ev != TL_EVT_TICK_EXPIRED) {
        return;
    }

    switch (fsm->state) {
    case TL_GREEN:
        enter_state(fsm, TL_YELLOW, now_ms);
        break;

    case TL_YELLOW:
        if (fsm->ped_pending) {
            fsm->ped_pending = false;
            enter_state(fsm, TL_RED_PED, now_ms);
        } else {
            enter_state(fsm, TL_RED, now_ms);
        }
        break;

    case TL_RED:
        enter_state(fsm, TL_ALL_RED, now_ms);
        break;

    case TL_RED_PED:
        enter_state(fsm, TL_ALL_RED, now_ms);
        break;

    case TL_ALL_RED:
        enter_state(fsm, TL_GREEN, now_ms);
        break;
    }
}

tl_state_t tl_fsm_state(const tl_fsm_t *fsm)
{
    return fsm->state;
}

const char *tl_fsm_state_name(tl_state_t s)
{
    switch (s) {
    case TL_GREEN:   return "GREEN";
    case TL_YELLOW:  return "YELLOW";
    case TL_RED:     return "RED";
    case TL_RED_PED: return "RED_PED";
    case TL_ALL_RED: return "ALL_RED";
    default:         return "?";
    }
}
