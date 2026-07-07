# Days 4 & 5 — Non-Blocking Timing (Traffic Light + LED Blinker)

Two projects in `traffic_lights/`:

- **Day 4** — state machine traffic light (GREEN → YELLOW → RED)
- **Day 5** — three independent LED blinkers, each on its own timer

Both use `HAL_GetTick()` instead of `HAL_Delay()` so the main loop never blocks.

---

# Day 4 — Traffic Light

## Pins used

| Color  | Pin  |
|--------|------|
| Green  | PB6  |
| Yellow | PA7  |
| Red    | PA6  |

## Concepts

### What are the three states in my traffic light program?

The three states are **GREEN**, **YELLOW**, and **RED**. Each state determines which light is on.

### What does each state physically do to the LEDs?

Each state turns off the previous LED and turns on the LED named by that state.

### What variable stores the current state?

`current_state` stores the current state.

### What variable stores when the current state started?

`last_change` stores when the current state started. It is a timestamp that marks the most recent state change.

### What does `HAL_GetTick()` return?

It returns the amount of time (in milliseconds) that has passed since the program started running.

### Why is `HAL_GetTick()` not the same thing as `HAL_Delay()`?

`HAL_GetTick()` does not freeze or stop the program — it is just a stopwatch. `HAL_Delay()` blocks the CPU and stops everything else from running.

### What does this expression mean?

```c
now - last_change >= duration
```

This checks whether enough time has passed since the last state change. When true, it is time to switch to the next state.

### Why do we set `last_change = now` after switching states?

We set it to `now` after switching because `last_change` acts as a timestamp for the latest change. Without updating it, the timer would immediately trigger again.

### Why should `set_green()`, `set_yellow()`, and `set_red()` turn the other LEDs off?

This is a traffic light system. If the other LEDs stayed on, we would not have three separate states — we would just have multiple lights on at once.

## Debug notes

### Why should `current_state` decide what comes next instead of using `last_change` to decide?

We cannot track which state comes next using `last_change` alone. `current_state` tells the program what phase we are in; `last_change` only tells us how long we have been in that phase.

### Why is the non-blocking traffic light better than the blocking `HAL_Delay()` version?

Using `HAL_Delay()` shuts off the whole program. It is like shutting off the AC in your car by cutting power to the entire car, instead of just pressing the AC button.

### What would happen if I forgot to update `last_change` after changing states?

Forgetting `last_change` would cause the state to change again immediately on the next loop iteration, so one LED would appear solid or flicker unpredictably.

### What would happen if I forgot to update `current_state` after changing the LED output?

Forgetting to update `current_state` would leave the state machine out of sync with the LEDs — for example, the green LED could stay solid or off after a change because the code still thinks it is in the wrong state.

---

# Day 5 — LED Blinker

## Concepts

### What is the difference between the traffic light project and the LED blinker project?

The traffic light project has three states that decide which single light is on. The LED blinker project gives each light its own timer and lets them blink independently.

### Why are the three blinkers not supposed to behave like a traffic light?

They act on a per-LED timer basis, not on a shared state machine. Each LED follows its own schedule.

### What does each `led_blinker` struct store?

Each struct stores:

- `port` — which GPIO port (e.g. `'B'`)
- `pin` — pin number on that port
- `interval_ms` — how long the LED stays in each on/off phase
- `last_change` — timestamp of the last toggle
- `state` — current software state (`GPIO_HIGH` or `GPIO_LOW`)

### Why does each LED need its own `last_change` timestamp?

Since each LED acts individually, each one needs its own `last_change` to track when it last toggled. Their blink times are separate.

### What would happen if all three LEDs shared one timestamp?

If all three LEDs shared one timestamp, they would all blink together at the same rate.

### What does `interval_ms` represent?

It represents how long the LED stays in its current on/off state before toggling.

### What does `state` represent inside the LED blinker struct?

It tracks the software state of the GPIO pin — `GPIO_HIGH` (voltage sent to the LED) or `GPIO_LOW` (no voltage).

### Why does `led_blinker_update()` toggle the LED instead of always setting it HIGH?

On each update, we flip the GPIO state so the LED blinks on and off. Always setting HIGH would leave the LED permanently on.

### What does this pattern mean?

```c
if (now - led->last_change >= led->interval_ms)
```

If the time since the last toggle is greater than or equal to the interval, it is time to switch the GPIO state.

### Why is there no `HAL_Delay()` inside `led_blinker_update()`?

Same reason as Day 4 — `HAL_Delay()` would block the whole program and prevent the other LEDs from updating.

### Why does `led_blinker_init()` set both the software state and the physical GPIO output?

The software `state` is for the GPIO abstraction layer. The physical GPIO output is what actually changes the voltage on the pin. Both need to match at startup.

## Debug notes

### What bug happened with `gpio_write()`?

`gpio_write()` was hardcoded to only work for Port A / PA5 (LD2).

### How did hardcoding `gpio_write()` to PA5/LD2 break the abstraction?

One of the LEDs was connected to **PB6**, so that LED would not blink even though the code compiled and ran. The higher-level blinker logic looked correct, but the low-level GPIO call was wrong.

### What does it mean to "generalize" `gpio_write()`?

Make it work for any port and pin, not just one hardcoded LED — so the abstraction scales to multiple GPIO outputs.

### Why should low-level GPIO functions be tested before blaming higher-level logic?

Low-level functions are what actually send voltage signals to the LEDs. If `gpio_write()` is wrong, no amount of state-machine or timer logic will fix the hardware behavior.

### How do I know my three LEDs are running independently?

Change the `interval_ms` values for each LED. If they blink at different rates, they are running on independent timers.
