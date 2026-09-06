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
| 6 | SPI + sensor drivers | Done | `Week_6_SPI_Sensors/` |
| 7 | Interrupts + debouncing | Done | `Week_7_Interrupts_Debounce/` |
| 8 | State machines + firmware architecture | Done | `Week_8_State_Machines/` |
| 9 | FreeRTOS basics | Done | `Week_9_FreeRTOS/` |
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

Application modules live inside full Nucleo-F446RE Cube/CMake projects (same layout as Weeks 3–5). See [`INTEGRATION.md`](INTEGRATION.md) for open/build/flash steps — CubeMX GUI is optional.

## Weeks 6–9 at a glance

### Week 6 — SPI + sensor drivers
Reusable SPI bus driver, HC-SR04 ultrasonic driver (kit sensor), layered architecture (bus → sensor → app), UART distance logging.

### Week 7 — Interrupts + debouncing
EXTI button ISR, software debounce, timer-based reaction game. Event-driven instead of `while(1) { poll(); }`.

### Week 8 — State machines + firmware architecture
Event-driven traffic light FSM with separated drivers, state logic, and application. Pedestrian request + non-blocking timers.

### Week 9 — FreeRTOS basics
Four tasks (sensor, OLED/display stub, serial log, heartbeat LED) with a queue and mutex. Focus on scheduling and synchronization, not RTOS internals.

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

## Board defaults used in later weeks

Unless a week README says otherwise:

| Function | Nucleo-F446RE |
|----------|----------------|
| ST-Link UART | USART2 (PA2 TX / PA3 RX) |
| User button | PC13 (active low, EXTI) |
| LD2 (green LED) | PA5 |
| I2C1 (OLED) | PB8 SCL / PB9 SDA |
| SPI1 | PA5 SCK / PA6 MISO / PA7 MOSI / PA4 NSS — *conflicts with LD2; Week 6 uses alternate pins* |
