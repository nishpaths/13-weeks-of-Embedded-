# Day 2 — Timer Interrupts & Event Flags

## Interrupt flow

How a timer interrupt ends up toggling the LED:

1. Timer counts
2. Timer reaches ARR
3. Update event happens
4. Timer requests an interrupt
5. CPU pauses `main`
6. Callback runs
7. Callback sets `event_occurred = true`
8. CPU returns to `main`
9. `main` sees the flag
10. LED toggles

```
timer counts
    ↓
reaches ARR → update event → interrupt request
    ↓
CPU pauses main → callback sets event_occurred
    ↓
CPU returns to main → main sees flag → LED toggles
```

## Why use a flag?

The interrupt callback should stay short. It just sets `event_occurred`, and `main` does the real work (toggle LED, count events, stop the timer) when it notices the flag.
