# Week 7 — Interrupts & debounce notes

## Why interrupts

Polling wastes CPU and couples timing to loop speed. Interrupts deliver **asynchronous** events: “button edge happened” / “timer expired.”

## Debouncing

Mechanical switches bounce. One physical press → many edges.

Strategy used here:

1. ISR records the first edge time.
2. After `settle_ms` (30 ms), main checks the pin is still active.
3. Only then fire one `confirmed_press`.

## Debugging practice

| Break | Symptom | Fix |
|-------|---------|-----|
| No pull-up / wrong EXTI edge | No events or inverted logic | Match active-low USER button |
| Debounce settle = 0 | Multiple presses per click | Increase settle_ms |
| UART inside EXTI | Hard faults / missed deadlines | Move print to main |
| Forget to enable NVIC | Silent failure | CubeMX NVIC checkboxes |
| Timer ARR never programmed | LED never turns on | Check `arm_random_timer` |
