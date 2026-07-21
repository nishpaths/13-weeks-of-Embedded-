# Day 3 — PWM and LED Brightness

## Goal

Understand how PWM works physically and use it to control LED brightness with an STM32 timer.

## What is PWM?

PWM stands for **Pulse Width Modulation**.

PWM controls the average power delivered to a device by rapidly switching a digital output fully ON and fully OFF.

The output pin is still switching between approximately:

- `0 V`
- `3.3 V`

It is **not** continuously outputting a lower voltage such as `1.65 V`.

The percentage of time that the signal stays ON during one PWM cycle is called the **duty cycle**.

---

## What determines PWM frequency?

PWM frequency is determined by:

- the timer input clock
- the prescaler
- the auto-reload value

The relationship is:

```
PWM frequency = timer clock / ((PSC + 1) × (ARR + 1))
```

### My setup

| Parameter    | Value   |
|--------------|---------|
| Timer clock  | 84 MHz  |
| PSC          | 83      |
| ARR          | 999     |

The prescaler reduces the timer clock to:

```
84 MHz / 84 = 1 MHz
```

This means the timer counter increments once every microsecond.

The timer counts from 0 to 999, which is 1000 counts total:

```
1000 counts × 1 µs = 1 ms
```

Therefore, the PWM frequency is:

```
1 / 1 ms = 1000 Hz
```

---

## What determines duty cycle?

Duty cycle is determined by the compare value relative to the total number of timer counts in one PWM period.

```
Duty cycle = Compare value / (ARR + 1) × 100%
```

With `ARR = 999`, there are 1000 counts per PWM period.

| Compare | Duty cycle |
|---------|------------|
| 0       | 0%         |
| 250     | 25%        |
| 500     | 50%        |
| 750     | 75%        |
| 1000    | 100%       |

The compare register controls how long the PWM output remains active during each timer period.

---

## Why does duty cycle change LED brightness?

Changing duty cycle changes the fraction of each PWM cycle during which current flows through the LED.

At 50% duty cycle:

- LED ON for 50% of the cycle
- LED OFF for 50% of the cycle

Because this switching happens very quickly, my eyes do not see individual flashes. Instead, I perceive the **average** light output.

- A **higher** duty cycle → LED receives power for more of each cycle → appears brighter
- A **lower** duty cycle → LED receives power for less of each cycle → appears dimmer

---

## What happens at 0% and 100% duty cycle?

**At 0% duty cycle:**

- The output is always inactive
- The LED is off

**At 100% duty cycle:**

- The output is always active
- The LED is fully on

The exact electrical state depends on PWM polarity, but with active-high PWM, 0% means always LOW and 100% means always HIGH.

---

## Why must PA0 use alternate-function mode?

TIM2 generates the PWM waveform internally on Channel 1.

PA0 supports the `TIM2_CH1` alternate function.

Alternate-function mode connects the internal timer channel output to the physical PA0 pin:

```
TIM2 → Channel 1 → alternate-function connection → PA0
```

If PA0 is configured as an ordinary GPIO output, the GPIO peripheral controls the pin instead of TIM2.

TIM2 may still generate PWM internally, but the waveform will not reach PA0.

---

## PWM frequency vs fade-update speed

**PWM frequency** controls how fast the output pin switches ON and OFF.

For my setup:

```
PWM frequency = 1000 Hz
```

This means the timer generates 1000 complete PWM cycles every second.

**Fade-update speed** controls how often the software changes the duty cycle.

For my fade:

```
Duty cycle update interval = 10 ms
```

This means the program changes the brightness percentage about 100 times per second.

These are separate rates:

- PWM frequency affects whether the LED appears steady or flickers
- Fade-update speed affects how quickly the brightness rises and falls

During one 10 ms fade update interval, a 1 kHz PWM signal completes approximately 10 PWM cycles.

---

## PWM abstraction

I created a `pwm_t` structure that stores:

- the timer handle
- the timer channel

My PWM module provides:

```c
void pwm_init(pwm_t *pwm, TIM_HandleTypeDef *htim, uint32_t channel);
void pwm_start(pwm_t *pwm);
void pwm_set_duty(pwm_t *pwm, uint8_t percentage);
```

The purpose of `pwm_set_duty()` is to let callers use a percentage instead of manually calculating compare values everywhere.

The function:

1. Limits the percentage to 100
2. Reads the timer auto-reload value
3. Converts the percentage into a compare value
4. Writes the compare value to the selected timer channel

---

## Non-blocking fade

The fade uses:

- a brightness percentage
- a direction boolean
- a stored timestamp
- a fade-update interval

The brightness increases from 0% to 100%, then reverses and decreases back to 0%.

The direction variable tracks whether the brightness is currently increasing or decreasing.

`HAL_GetTick()` is used instead of `HAL_Delay()` so the main loop can continue running while the fade timing is handled.

The timing pattern is:

```
now = HAL_GetTick()

if now - last_change >= interval:
    update last_change
    change brightness
    update PWM duty cycle
```

---

## Intentional failure tests

### Wrong PWM channel

**Result:** The LED did not respond correctly because PWM was generated on a different timer channel.

**Lesson:** The selected timer channel must match the channel connected to the physical PWM pin.

### PA0 configured as ordinary GPIO

**Result:** The LED did not respond to PWM duty-cycle changes.

**Lesson:** The pin must be placed in alternate-function mode so the timer peripheral can control it.

### Very low PWM frequency

**Expected result:** The LED visibly flickers instead of appearing steadily dim.

**Lesson:** The PWM signal must switch fast enough that individual ON and OFF cycles are not noticeable.

### Compare value above the period

**Expected result:** The PWM output may remain active for the entire timer cycle or behave unexpectedly.

**Lesson:** The compare value should remain within the valid range defined by the timer period.

---

## Final takeaway

PWM brightness control uses two separate timing layers:

1. The **hardware timer** rapidly switches the LED ON and OFF
2. The **software** slowly changes the duty cycle to create the fade

The timer handles the fast PWM waveform automatically, while the main loop only updates the requested brightness percentage.
