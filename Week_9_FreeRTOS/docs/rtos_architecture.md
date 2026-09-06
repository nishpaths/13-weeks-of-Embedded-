# Week 9 — RTOS architecture

```
                    ┌─────────────────┐
                    │  FreeRTOS       │
                    │  scheduler      │
                    └────────┬────────┘
           ┌─────────┬───────┼───────┬─────────┐
           ▼         ▼       ▼       ▼         │
      SensorTask  Display  LogTask  Heartbeat  │
           │         ▲       ▲          │      │
           │         │       │          │      │
           │    qDisplay   qLog         │      │
           └─────────┴───────┘          │      │
                                        │      │
                              mtxUart ──┴──────┘
                                 │
                            USART2 / LD2
```

## Responsibilities

| Piece | Role |
|-------|------|
| SensorTask | Producer — samples world, never prints |
| DisplayTask | Consumer — UI path (OLED stub via UART tag) |
| LogTask | Consumer — debug telemetry |
| HeartbeatTask | Liveness indicator; lowest priority |
| qDisplay / qLog | Decouple producer rate from consumers |
| mtxUart | Serialize UART so lines don’t interleave |

## Why not one giant loop?

A single loop couples sensor latency to display and logging. With tasks, a slow UART flush does not stop the heartbeat LED if priorities and blocking are correct.
