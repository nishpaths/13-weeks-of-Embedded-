# ISR documentation

## EXTI — USER button (PC13)

| Item | Detail |
|------|--------|
| Trigger | Falling edge (button pressed, active low) |
| Handler path | `EXTI15_10_IRQHandler` → HAL → `HAL_GPIO_EXTI_Callback` |
| Work in ISR | `events_set_button`, `debounce_on_isr_edge` |
| Forbidden in ISR | `HAL_UART_Transmit`, long loops, `malloc` |

**Latency:** EXTI latency is typically microseconds. Bounce lasts ~5–20 ms — hence software debounce in main, not “trust every edge.”

## TIM3 — random “go” delay

| Item | Detail |
|------|--------|
| Mode | One-shot style: start IT, stop in callback |
| Work in ISR | Stop timer, `events_set_timer` |
| Meaning | LED should turn on; enter `RG_WAIT_PRESS` |

## Shared data

`event_flags_t` fields are `volatile` because main and ISRs share them. For single-byte/bool flags on Cortex-M this is enough; for larger shared structs use critical sections or RTOS primitives (Week 9).

## False starts

A confirmed press during `RG_WAIT_RANDOM` is a false start — player jumped the gun before the LED.
