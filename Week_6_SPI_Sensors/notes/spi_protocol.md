# SPI protocol notes

## What SPI is

SPI is a **synchronous** serial bus: the master provides SCLK; data moves on MOSI/MISO on clock edges.

Typical 4-wire master mode:

| Line | Role |
|------|------|
| SCLK | Clock from master |
| MOSI | Master → slave |
| MISO | Slave → master |
| CSn  | Chip select (usually active low) |

## Clock polarity / phase (CPOL / CPHA)

| Mode | CPOL | CPHA | Idle clock | Sample edge |
|------|------|------|------------|-------------|
| 0 | 0 | 0 | Low | Rising |
| 1 | 0 | 1 | Low | Falling |
| 2 | 1 | 0 | High | Falling |
| 3 | 1 | 1 | High | Rising |

**Wrong mode = garbage data** even if wiring is perfect. Always match the slave datasheet.

## Master / slave

- Master owns the clock and asserts CS.
- Only one slave should be selected at a time on a shared bus.
- Deselect CS between transactions unless the datasheet requires a continuous frame.

## Driver architecture takeaway

Wrap HAL in a bus driver (`spi_bus`) so sensor code says `read_reg(0x2D)` instead of managing CS + `HAL_SPI_TransmitReceive` everywhere.

## Logic analyzer checklist

Capture SCLK, MOSI, MISO, CS and verify:

1. CS goes low before first clock, high after last
2. Clock idle level matches CPOL
3. First data bit aligns with the expected CPHA edge
4. Byte order is MSB-first (most sensors) unless datasheet says otherwise
