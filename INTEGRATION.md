# How to build Weeks 6–9

Each week now contains a **full Nucleo-F446RE Cube/CMake project** (same style as Weeks 3–5), with peripherals initialized in `main.c` / `stm32f4xx_hal_msp.c`.

You do **not** need to re-run CubeMX unless you want to change pins.

## Option A — STM32CubeIDE

1. File → Open Projects from File System → select the week’s project folder  
   (e.g. `Week_6_SPI_Sensors/Ultrasonic_SPI_Project`).
2. Build, then Run/Debug on the Nucleo.
3. Serial monitor at **115200** on the ST-Link COM port.

## Option B — CMake (same as Week 4)

From the project folder, use your existing `CMakePresets.json` / arm-none-eabi toolchain preset.

## Pin cheat sheet

| Week | Key pins |
|------|----------|
| 6 | TRIG PB0, ECHO PB1, SPI2 PB13/14/15, CS PB12, UART PA2/3 |
| 7 | LD2 PA5, USER PC13, TIM3, UART PA2/3 |
| 8 | R PA6, Y PA7, G PB6, PED PB10, USER PC13 |
| 9 | Same sonar as week 6 + LD2 PA5 + FreeRTOS |

**Always level-shift HC-SR04 ECHO to 3.3 V.**
