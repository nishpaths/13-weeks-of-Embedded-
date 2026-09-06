# Week 6 — Driver architecture

## Layering

```
┌─────────────────────────────────────────┐
│              Application                 │
│   main.c — sample rate, UART logging     │
└───────────────┬─────────────────────────┘
                │
        ┌───────┴────────┐
        │                │
┌───────▼──────┐  ┌──────▼──────────┐
│   hcsr04     │  │  spi_regdev     │
│ sensor logic │  │ sensor-shaped   │
└───────┬──────┘  └──────┬──────────┘
        │                │
        │         ┌──────▼──────────┐
        │         │    spi_bus      │  ← hardware abstraction
        │         └──────┬──────────┘
        │                │
┌───────▼──────┐  ┌──────▼──────────┐
│ GPIO + TIM2  │  │   HAL SPI1      │
│ (TRIG/ECHO)  │  │   + CS GPIO     │
└──────────────┘  └─────────────────┘
```

## Separation rules

| Layer | Owns | Must not own |
|-------|------|----------------|
| `spi_bus` | CS framing, transfers, timeouts | Sensor register maps |
| `hcsr04` / `spi_regdev` | Datasheet timing / registers | UART printing, business policy |
| Application | When to sample, what to display | Bit-banging SPI or echo timing |

## Why this matters

If TRIG timing changes, only `hcsr04.c` changes. If you swap SPI sensors, keep `spi_bus` and rewrite the sensor layer. Application stays thin.
