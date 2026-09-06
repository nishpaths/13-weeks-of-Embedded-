# Task interaction

```
SensorTask                 DisplayTask              LogTask
    │                           │                      │
    │  sample every 200 ms      │                      │
    ├──── put(qDisplay) ───────►│                      │
    ├──── put(qLog) ──────────────────────────────────►│
    │                           │ get                  │ get
    │                           │ format [OLED]        │ format [LOG]
    │                           │                      │
    │                      acquire mtxUart             │
    │                           │ print                │ print
    │                      release mtxUart             │

HeartbeatTask: toggle LD2 + "[HB] alive" every 500 ms (also uses mtxUart)
```

## Priority intuition

- Equal Normal priority: Sensor vs Display share CPU fairly (round-robin time slicing if enabled).
- Log at BelowNormal: telemetry yields to sensing/UI under load.
- Heartbeat at Low: starves only if higher tasks never block — they `osDelay` / queue-wait so HB still runs.

## Failure modes to watch

| Bug | What you observe |
|-----|------------------|
| No mutex on UART | Garbled mixed lines |
| Queue length 1 + never reading | Sensor `put` fails; stale UI |
| `HAL_Delay` in task instead of `osDelay` | Blocks whole system (SysTick shared carefully — prefer `osDelay`) |
| Blocking sensor read forever | Display/Log starve for new data |
