# Week 7 — Interrupt-driven architecture

```
          EXTI (PC13)                    TIM3 update
               │                              │
               ▼                              ▼
     reaction_game_on_exti          reaction_game_on_timer
               │                              │
               ▼                              ▼
        events.button_edge             events.timer_fired
        debounce pending edge
               │                              │
               └──────────┬───────────────────┘
                          ▼
                 reaction_game_update()   ← main loop only
                          │
                          ▼
              state machine + LED + UART result
```

## Rules

1. **ISRs stay tiny** — set a flag / record a timestamp, never print UART or run game logic.
2. **Main owns policy** — debounce confirmation, scoring, LED patterns.
3. **No busy-polling the button** as the primary path — EXTI wakes the event path; main only samples the pin to confirm after settle time.

## Avoid this

```c
while (1) {
    if (button_pressed()) { ... }  // primary control path
}
```
