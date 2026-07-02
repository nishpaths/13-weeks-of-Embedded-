# Day 2 — GPIO Concepts & Abstraction

## Concepts

### What is a GPIO port?

A GPIO port is a group of pins controlled by that GPIO register.

### What is a GPIO pin?

A GPIO pin is the place where we can connect peripherals. You can configure that pin so it can act as:

- **Input** — read a voltage level from the outside world
- **Output** — drive a voltage level (what I used today for the LED)
- **Alternate function** — route the pin to a peripheral like UART or SPI
- **Analog** — connect to the ADC

### What does PA5 mean?

`PA5` is pin 5 inside GPIO port A.

| Symbol | Meaning |
|--------|---------|
| `P`    | Pin |
| `A`    | GPIO port A |
| `5`    | Pin number 5 on that port |


### What does output mode mean?

Output mode means the MCU pin is configured to **drive** a voltage level instead of reading one.

### How does one bit map to one pin?

A GPIO port has registers where individual bits control individual pins.

### What did my abstraction hide?

- The direct HAL calls from `main.c`

---

## Bugs I hit

See debugging notes below.

## Debugging notes

### 1. Calling the wrong GPIO pin

`gpio_toggle('A', 6)` led to the LED not blinking at all because the pin was wrong. The LED is on **PA5**, not PA6.

### 2. Removing `gpio.h` from `main.c`

Gives a compiler error because the GPIO helper functions are no longer declared. Header files are like libraries and templates — they tell the compiler what exists before the linker connects to the actual code.

### 3. Including HAL in `gpio.h`

Caused huge HAL type errors. Keep HAL includes in `.c` files when possible, and use forward declarations or minimal types in headers.
