# Week 6 — SPI + Sensor Drivers

**Board:** STM32 Nucleo-F446RE  
**Project:** `Ultrasonic_SPI_Project/` (full Cube/CMake tree — open or build like Weeks 3–5)

## Peripherals (already wired in code)

| Peripheral | Config |
|------------|--------|
| USART2 | PA2/PA3, 115200 — ST-Link VCP |
| TIM2 | 1 MHz free-running (µs timer for HC-SR04) |
| SPI2 | PB13 SCK, PB14 MISO, PB15 MOSI, Mode 0, soft NSS |
| GPIO | PB12 = SPI CS, PB0 = TRIG, PB1 = ECHO |

## Wiring

| Signal | Nucleo pin | Notes |
|--------|------------|--------|
| HC-SR04 VCC | 5V | |
| HC-SR04 GND | GND | |
| HC-SR04 TRIG | **PB0** | |
| HC-SR04 ECHO | **PB1** | **Level-shift 5V → 3.3V** |
| SPI SCK/MISO/MOSI | PB13/PB14/PB15 | Optional SPI device |
| SPI CS | PB12 | Active low |
| Serial | ST-Link USB | 115200 |

## Build / flash

Same CMake flow as your Week 4 projects, or open the folder in STM32CubeIDE.

```text
distance: 42.0 cm
```

## Deliverables

- Reusable `spi_bus` + `hcsr04` drivers
- Architecture: `docs/architecture.md`
- Notes: `notes/spi_protocol.md`, `notes/debugging_notes.md`
