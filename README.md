# 13-Week Embedded Systems + Hardware Summer Plan

Roadmap from “I know some C” to building and debugging real embedded systems on an **STM32 Nucleo-F446RE**.

Philosophy: **depth > breadth**. Build independently, read datasheets, and debug on purpose.

## Hardware & tools

| Item | Notes |
|------|--------|
| STM32 Nucleo-F446RE | Main board (ARM Cortex-M4) |
| Breadboard, jumpers, LEDs, buttons, resistors, pot | Basic I/O |
| I2C OLED | Week 5+ |
| Ultrasonic sensor (HC-SR04) | Week 6 sensor driver |
| Logic analyzer + PulseView | From Week 5 |
| KiCad | From Week 9/10 |

**Software:** VS Code or STM32CubeIDE, Git, serial monitor. FreeRTOS via CubeMX for Week 9.

## Roadmap

| Week | Topic | Status | Folder |
|------|--------|--------|--------|
| 1 | Embedded C fundamentals | Done | `Week_1_C/` |
| 2 | Advanced C + memory thinking | Done | `Week_2_Advanced_Embedded_C/` |
| 3 | Electronics + GPIO | Done | `Week_3_Electronics_+_GPIO/` |
| 4 | Timers, PWM, UART | Done | `Week_4_Timers_PWM_UART/` |
| 5 | I2C + logic analyzer | Done | `Week_5_I2C/` |
| 6 | SPI + sensor drivers | Planned | — |
| 7 | Interrupts + debouncing | Done | `Week_7_Interrupts_Debounce/` |
| 8 | State machines + firmware architecture | Done | `Week_8_State_Machines/` |
| 9 | FreeRTOS basics | Planned | — |
| 10 | PCB design (KiCad) | Planned | — |
| 11 | Final project architecture | Planned | — |
| 12 | Final project completion | Planned | — |
| 13 | Refactor + portfolio polish | Planned | — |

## Weekly time target

Aim for **15–25 focused hours/week**: ~40% learning, ~60% building and debugging.

## How this repo is organized

Each week folder typically has:

- **Notes** — protocol concepts, debugging lessons, datasheet takeaways
- **Firmware** — modular C drivers (HAL abstraction → sensor/app logic)
- **Architecture docs** — diagrams of layering and control flow
- **Wiring** — pin maps for Nucleo-F446RE

## How I used AI

- AI helped build this roadmap for me.
- I used ChatGPT as a tutor — to explain concepts and guide debugging — not to hand me finished code or answers. I also give it the STM32 datasheet so it's easier for me to look into and see where I need to make changes whenever I'm configuring my pins.
- I used Cursor to format my notes and clean up Markdown so everything reads more clearly.
- I also used Cursor to commit and push changes to this repo.

## Recommended final project (Weeks 11–12)

**Smart Environmental Monitor:** multiple sensors, OLED, interrupts, RTOS tasks, menu, logging — with at least one subsystem built primarily from a datasheet.

## Outcome

By the end of this roadmap you should be able to:

- Write embedded C comfortably and structure firmware cleanly
- Debug hardware/software interactions
- Use GPIO, timers, interrupts, UART, I2C, and SPI
- Read datasheets effectively
- Apply basic RTOS concepts
- Discuss engineering tradeoffs in interviews
