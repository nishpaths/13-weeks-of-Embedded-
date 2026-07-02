First STM32 blink test:
- Board: NUCLEO-F446RE
- LED: LD2
- Pin: PA5
- Peripheral: GPIOA
- CubeMX config: PA5 as GPIO_Output
- Code used: HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin)
- Delay test: changed HAL_Delay values and confirmed blink speed changed
- Debug issue: ST-LINK not detected after update; fixed by unplug/replug board

# Day 1 — LED Basics

## Circuit

Final GPIO-controlled circuit:

PA5 / D13 → resistor → LED anode  
LED cathode → GND

## What I proved

At first, I powered the LED directly from 3.3V and GND. That proved the LED, resistor, and polarity worked, but it did not prove GPIO control.

Then I moved the circuit to PA5 / D13. My STM32 code toggled PA5 using HAL_GPIO_TogglePin(), and the external LED blinked. This proved that the GPIO pin was driving the circuit.

## GPIO behavior

When PA5 is HIGH, the pin is near 3.3V and current flows through the resistor and LED to ground.

When PA5 is LOW, the pin is near 0V and the LED turns off.

## Debugging tests

1. Flipped LED backward: LED did not blink. Fixed by restoring polarity.
2. Moved wire to wrong pin: external LED did not blink. Fixed by reconnecting to D13 / PA5.
3. Disconnected GND: LED did not blink. Fixed by reconnecting ground.

## Key lesson

A GPIO pin is a physical electrical node. Code changes the pin voltage, and the external circuit determines whether current flows.