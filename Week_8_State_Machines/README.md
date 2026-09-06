# Week 8 — State Machines + Firmware Architecture

**Board:** STM32 Nucleo-F446RE  
**Project:** `Traffic_Light_FSM/` (Cube/CMake project for Nucleo-F446RE)

## Wiring

| Function | Pin |
|----------|-----|
| Red LED | PA6 |
| Yellow LED | PA7 |
| Green LED | PB6 |
| Pedestrian LED | PB10 |
| Ped / USER button | PC13 |
| Serial | USART2 115200 |

## Behavior

Cycle: GREEN → YELLOW → RED (or RED_PED if button was pressed) → ALL_RED → GREEN.

Press USER anytime; the request is latched and honored after yellow.

## Deliverables

- Layered FSM (`tl_gpio` / `tl_events` / `tl_fsm`)
- `docs/architecture.md`, `docs/state_machine.md`
