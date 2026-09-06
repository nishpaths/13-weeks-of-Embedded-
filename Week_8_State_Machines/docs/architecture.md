# Week 8 — Firmware architecture

```
┌──────────────────────────────────────────────┐
│ Application (main.c)                         │
│  - init timing/pins                          │
│  - call tl_fsm_update each loop              │
│  - UART log on state change                  │
└───────────────────┬──────────────────────────┘
                    │
┌───────────────────▼──────────────────────────┐
│ State logic (tl_fsm.c)                       │
│  - states, transitions, ped_pending          │
│  - owns WHEN lamps change, not HOW           │
└─────────┬───────────────────────┬────────────┘
          │                       │
┌─────────▼─────────┐   ┌─────────▼────────────┐
│ Events (tl_events)│   │ Drivers (tl_gpio)    │
│ ped request latch │   │ set red/yel/grn/ped  │
└─────────┬─────────┘   └─────────┬────────────┘
          │                       │
     EXTI / GPIO              HAL_GPIO_WritePin
```

## Refactor wins vs Week 3

| Before (Week 3) | After (Week 8) |
|-----------------|----------------|
| `if/else` chain mixed with GPIO | Named states + `enter_state` |
| No pedestrian path | `ped_pending` event across yellow→red |
| Timing constants inside update | `tl_timing_t` config struct |
| Hard to test transitions | Events can be injected without hardware |

## Naming & modularity

- Prefix modules `tl_` (traffic light) to avoid collisions.
- Headers expose behavior, not HAL details (except pin types needed for init).
- Application never calls `HAL_GPIO_WritePin` for lamps directly.
