# Traffic light state machine

## State diagram

```
                 timeout
   ┌────────── GREEN ──────────┐
   │                           ▼
   │                        YELLOW
   │                      /        \
   │            ped_pending         no ped
   │                 │                │
   │                 ▼                ▼
   │             RED_PED             RED
   │                 \              /
   │                  \            /
   │                   ▼          ▼
   │                   ALL_RED
   │                      │
   └──────────────────────┘
```

## Transition table

| Current | Event | Next | Notes |
|---------|-------|------|-------|
| GREEN | tick expired | YELLOW | Always |
| YELLOW | tick expired + ped pending | RED_PED | Clears ped latch |
| YELLOW | tick expired | RED | Normal cycle |
| RED | tick expired | ALL_RED | Clearance |
| RED_PED | tick expired | ALL_RED | End walk |
| ALL_RED | tick expired | GREEN | Restart |

Pedestrian request can arrive in any state; it is **latched** (`ped_pending`) and consumed when leaving YELLOW.

## Timing defaults

| State | ms |
|-------|-----|
| GREEN | 5000 |
| YELLOW | 2000 |
| RED | 4000 |
| RED_PED | 5000 |
| ALL_RED | 1000 |

Tune via `tl_timing_t` — do not hardcode inside the switch.

## Non-blocking design

No `HAL_Delay` in the FSM. Each state records `state_entered_ms`; expiration is computed from `HAL_GetTick()`.
