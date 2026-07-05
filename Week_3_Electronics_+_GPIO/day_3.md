# Day 3 — Button Input + Pull-Up/Pull-Down Resistors

## Goal

The goal for today was to read a button input correctly and understand why GPIO inputs need a defined electrical state. A GPIO input reads voltage, not “button pressed” directly.

For STM32 GPIO:

* LOW means the pin is near 0V.
* HIGH means the pin is near 3.3V.
* If a pin is not connected to a defined voltage, it can float and randomly read HIGH or LOW.

## Pins Used

* LED: PA5 / LD2
* Button: PC13 / B1 USER button

## Final Pin Configuration

* PA5 configured as GPIO output.
* PC13 configured as GPIO input.
* PC13 pull setting: GPIO_NOPULL.

I used `GPIO_NOPULL` because the onboard Nucleo USER button circuit already provided stable behavior.

## Input vs Output

An output pin drives a voltage.

Example:

* PA5 drives the LED HIGH or LOW.

An input pin reads a voltage.

Example:

* PC13 reads the voltage created by the USER button circuit.

The LED pin should be output because the MCU controls it.
The button pin should be input because the MCU only reads it.

## Pull-Up Button

A pull-up button has a default HIGH state.

When not pressed:

* The resistor pulls the GPIO pin HIGH.

When pressed:

* The button connects the GPIO pin to GND.
* The pin reads LOW.

This means the logic is active-low:

* Not pressed = HIGH
* Pressed = LOW

Diagram:

```text
3.3V
 |
 resistor
 |
 GPIO input
 |
 button
 |
GND
```

## Pull-Down Button

A pull-down button has a default LOW state.

When not pressed:

* The resistor pulls the GPIO pin LOW.

When pressed:

* The button connects the GPIO pin to 3.3V.
* The pin reads HIGH.

Diagram:

```text
3.3V
 |
 button
 |
 GPIO input
 |
 resistor
 |
GND
```

## Which One I Used

For the onboard Nucleo USER button on PC13, the behavior was active-low.

Observed behavior:

* Button not pressed: LED off.
* Button pressed: LED on.

The code checks for `GPIO_LOW` to detect a press:

```c
if (gpio_read('C', 13) == GPIO_LOW)
{
    gpio_write('A', 5, GPIO_HIGH);
}
else
{
    gpio_write('A', 5, GPIO_LOW);
}
```

This means:

* `GPIO_LOW` on PC13 means the button is pressed.
* `GPIO_HIGH` on PC13 means the button is not pressed.

## Why Button Logic May Be Inverted

Button logic can be inverted because the GPIO input only reads voltage.

With pull-up logic, the pin is normally HIGH. Pressing the button connects the pin to ground, so the pin reads LOW. Therefore, LOW can mean “pressed.”

This is different from a pull-down button, where pressing the button makes the pin HIGH.

## Floating Inputs

A floating input happens when a GPIO input pin is not connected to a defined HIGH or LOW voltage.

If there is no pull-up, pull-down, or external circuit defining the voltage, the input may randomly read HIGH or LOW. This is dangerous because the program may think the button was pressed even when it was not.

Important rule:

A GPIO input must have a defined state when the button is not pressed.

## GPIO Driver Functions Added

I added input support to my GPIO driver.

Functions used:

```c
void gpio_output_init(char port, uint8_t pin);
void gpio_input_init(char port, uint8_t pin, gpio_pull_t pull);
void gpio_write(char port, uint8_t pin, gpio_state_t state);
gpio_state_t gpio_read(char port, uint8_t pin);
```

The input init function configures a pin as input and applies the selected pull setting.

The read function returns the electrical state of the pin:

* `GPIO_LOW`
* `GPIO_HIGH`

It does not return “pressed” or “not pressed.” Button meaning is handled in the main application logic.

## Pin Mask Reminder

The GPIO driver converts a pin number into a pin mask using:

```c
(uint16_t)(1U << pin)
```

Example:

```c
gpio_read('C', 13);
```

uses:

```c
1U << 13
```

This creates a mask for pin 13, which is equivalent to using `GPIO_PIN_13`.

HAL wants a pin mask, not the raw number 13.

## Bugs / Debugging Notes

### 1. Confused input mode with pull setting

At first, I thought `GPIO_Input` and pull-up/pull-down were the same type of setting.

Correction:

* GPIO mode controls whether the pin reads or drives.
* Pull setting controls the default voltage state.

### 2. Button pin as output

The button pin should not be configured as output because the MCU is supposed to read the button circuit, not drive it.

Correct setup:

* PC13 = input
* PA5 = output

### 3. Active-low confusion

The USER button is active-low, so pressed means the pin reads LOW.

Correct logic:

```c
if (gpio_read('C', 13) == GPIO_LOW)
{
    // button pressed
}
```

### 4. Header/prototype issue

I got an implicit declaration error for `gpio_output_init`.

Cause:

* The function existed in `gpio.c`, but `main.c` did not know about it properly.

Fix:

* Add the function prototype to `gpio.h`.
* Include `gpio.h` in `main.c`.

### 5. Clock enable mistake

When initializing GPIO ports, the port and clock must match.

Correct examples:

```c
gpio_port = GPIOA;
__HAL_RCC_GPIOA_CLK_ENABLE();

gpio_port = GPIOC;
__HAL_RCC_GPIOC_CLK_ENABLE();
```

Wrong example:

```c
gpio_port = GPIOC;
__HAL_RCC_GPIOA_CLK_ENABLE();
```

## Final Result

The final program used my GPIO abstraction instead of direct HAL calls in the main loop.

Final behavior:

* Button not pressed: LED off.
* Button pressed: LED on.

This confirmed that:

* `gpio_output_init()` works.
* `gpio_input_init()` works.
* `gpio_write()` works.
* `gpio_read()` works.
* PC13 reads the USER button.
* PA5 controls LD2.

## What I Learned

GPIO input reads voltage, not button intent.

A button circuit must define what the input reads when the button is not pressed.

Pull-up logic is often active-low, meaning LOW can mean pressed.

The GPIO driver should return electrical state, not button meaning.

A GPIO abstraction hides HAL details, but I still need to understand what HAL is configuring underneath.
