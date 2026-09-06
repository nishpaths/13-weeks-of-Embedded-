# Week 8 notes — firmware architecture

## Finite state machines in embedded

An FSM makes illegal states/transitions harder. Prefer:

```c
switch (state) {
  case A: /* handle events, maybe enter B */ break;
}
```

over deeply nested flags (`if (green && !ped && timer)`).

## Event-driven vs polled

- **Events:** something happened (button, timeout).
- **State:** what the system *is* right now.
- **Actions:** outputs applied on entry (lamps).

## Debugging practice

1. Log every state entry over UART (already in `main.c`).
2. Force a ped request mid-green — confirm it waits until after yellow.
3. Break timings (yellow = 50 ms) and observe race with bounce — debounce still matters.
4. Remove `ALL_RED` and note why clearance gaps exist in real intersections.
