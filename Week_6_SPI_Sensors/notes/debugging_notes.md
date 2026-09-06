# Week 6 — Debugging notes

## Intentional breaks (practice these)

### SPI timing / mode
- **Symptom:** WHO_AM_I always 0x00 or 0xFF.
- **Cause:** Wrong CPOL/CPHA, or baud too fast for breadboard.
- **Fix:** Match datasheet mode; start ≤ 1 MHz; check with logic analyzer.

### Chip select logic
- **Symptom:** Bus looks active on analyzer but device never responds.
- **Cause:** CS left high, or shared with another GPIO, or active-high vs active-low mismatch.
- **Fix:** Confirm CS polarity; scope CS relative to SCLK; only one slave selected.

### Initialization order
- **Symptom:** First transfer fails; later ones work (or vice versa).
- **Cause:** SPI clock not enabled, GPIO AF not set, timer for HC-SR04 not started before first ping.
- **Fix:** `HAL_SPI_Init` / `MX_SPI1_Init` before transfers; `HAL_TIM_Base_Start` before `hcsr04_read_*`.

### HC-SR04 specific
| Break | What you see |
|-------|----------------|
| ECHO still at 5V into MCU | Flaky reads or damaged pin — use divider |
| TRIG pulse < 10 µs | Frequent timeouts |
| Timeout too short | Valid far targets look like timeout |
| Blocking echo wait forever | Main loop freezes — always use timeout_us |

## Logic analyzer habits

1. Trigger on CS falling (SPI) or TRIG rising (ultrasonic).
2. Measure TRIG width — should be ≥ 10 µs.
3. Measure ECHO high time — divide by 58 for cm; sanity-check against a ruler.
4. For SPI, decode MOSI/MISO as hex and compare to expected register values.
