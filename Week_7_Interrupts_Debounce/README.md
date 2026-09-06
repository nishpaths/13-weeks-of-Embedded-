# Week 7 — Interrupts + Debouncing

**Board:** STM32 Nucleo-F446RE  
**Project:** `Reaction_Game/` (full Cube/CMake project)

## Peripherals (already wired)

| Peripheral | Config |
|------------|--------|
| USART2 | PA2/PA3, 115200 |
| TIM3 | Update interrupt — random “go” delay |
| PC13 | EXTI falling — USER button |
| PA5 | LD2 output |

## How to play

1. Flash and open serial at 115200.
2. Wait for LD2 to turn on (after a random 1–4 s delay).
3. Press the blue USER button.
4. Serial prints `Reaction: N ms`. Pressing early → `False start!`

## Deliverables

- Debounce + interrupt-driven game logic
- `docs/architecture.md`, `docs/isr_notes.md`


