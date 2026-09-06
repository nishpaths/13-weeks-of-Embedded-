# Week 9 — FreeRTOS Basics

**Board:** STM32 Nucleo-F446RE  
**Project:** `MultiTask_System/` (full Cube/CMake tree + FreeRTOS middleware)

## Tasks

| Task | Role |
|------|------|
| SensorTask | HC-SR04 every 200 ms → two queues |
| DisplayTask | `[OLED] ...` (UART stub for OLED) |
| LogTask | `[LOG] dist=...` |
| HeartbeatTask | Toggle LD2 (PA5); `[HB] alive` every 2 s |

UART access is protected by a mutex.

## Wiring

| Signal | Pin |
|--------|-----|
| HC-SR04 TRIG | PB0 |
| HC-SR04 ECHO | PB1 (3.3V level-shifted) |
| LD2 | PA5 (on-board) |
| Serial | USART2 / ST-Link |

Bring-up without sonar: in `app_freertos.c` set `#define USE_REAL_SONAR 0`.

## Deliverables

- `docs/rtos_architecture.md`, `docs/task_interaction.md`
- `notes/synchronization.md`
