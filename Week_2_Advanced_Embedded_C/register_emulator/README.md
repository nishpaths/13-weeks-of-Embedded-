# Register Emulator

A console program that simulates **memory-mapped I/O (MMIO)** registers — the same pattern real microcontrollers use to expose hardware control to firmware.

## Why this exists

On embedded hardware, registers are not ordinary variables. They live at fixed memory addresses and have **access rules**:

- Some registers are **read/write** (you configure them freely).
- Some are **read-only** (hardware updates them; writes are ignored or rejected).
- Some are **write-only** (you send commands; reads are meaningless or forbidden).

This emulator trains that model before you work with real datasheets and peripheral drivers.

## MMIO system

The simulator models a small MCU with four 8-bit registers:

| Register | Access       | Role |
|----------|--------------|------|
| `PORTA`  | Read/Write   | General-purpose I/O port |
| `DDRB`   | Read/Write   | Data direction register |
| `STATUS` | Read-only    | Hardware status flags |
| `CMD`    | Write-only   | Command register |

Each register is a struct:

```c
typedef struct {
    uint8_t value;
    ACCESS_TYPE access_type;
    char name[50];
} Register;
```

Access is enforced in software — the same discipline you need when writing drivers against real hardware.

## Register bits

Named bit positions mirror common peripheral flags:

| Bit | Name           |
|-----|----------------|
| 0   | `LED_ENABLE`   |
| 1   | `MOTOR_ENABLE` |
| 2   | `ERROR_FLAG`   |
| 3   | `TX_READY`     |
| 4   | `RX_READY`     |
| 5   | `POWER_SAVE`   |
| 6   | `INTERRUPT_EN` |
| 7   | `SYSTEM_READY` |

Bit operations use the same masks as Week 1:

```c
reg->value |= (1 << bit);   /* set bit */
reg->value &= ~(1 << bit);  /* clear bit */
(reg->value & (1 << bit))   /* read bit */
```

## API

| Function | Description |
|----------|-------------|
| `read_register(reg)` | Returns full byte; blocked on write-only registers |
| `write_register(reg, value)` | Writes full byte; blocked on read-only registers |
| `set_bit(reg, bit)` | Sets one bit; blocked on read-only registers |
| `clear_bit(reg, bit)` | Clears one bit; blocked on read-only registers |
| `read_bit(reg, bit)` | Returns 0 or 1; blocked on write-only registers |
| `print_reg(reg)` | Prints register name and binary value |

## Build and run

```bash
gcc main.c -o main
./main
```

On Windows with MinGW:

```powershell
gcc main.c -o main.exe
.\main.exe
```

## What the demo tests

`main()` runs a sequence of scenarios that show access control in action:

1. **Write to `PORTA`** — `0xAA` is stored and printed as `10101010`.
2. **Write to `STATUS`** — rejected (`Register is read only`); value stays `0`.
3. **Write to `CMD`** — `0x55` is accepted; register holds the value internally.
4. **Read from `CMD`** — rejected (`Register is write only`); returns `0`.
5. **Set bit on `STATUS`** — rejected; read-only registers cannot be modified via bit ops.
6. **Set `LED_ENABLE` on `PORTA`** — bit 0 is set on the read/write register.
7. **Read `SYSTEM_READY` from `STATUS`** — allowed on a read-only register.
8. **Read bit from `CMD`** — rejected; write-only registers hide their contents.

## Example output

```
PORTA: 0b00000000
DDRB: 0b00000000
STATUS: 0b00000000
CMD: <write-only register>

Writing 0xAA to PORTA
PORTA: 0b10101010

Trying to write 0xFF to STATUS
Register is read only
STATUS: 0b00000000

Writing 0x55 to CMD
CMD: <write-only register>

Trying to read CMD
Register is write only
CMD read returned: 0

Trying to set SYSTEM_READY on STATUS
Register is read only
STATUS: 0b00000000

Setting LED_ENABLE on PORTA
PORTA: 0b10101011

Reading STATUS bit SYSTEM_READY
STATUS SYSTEM_READY: 0

Trying to read bit from CMD
Register is write only
CMD LED_ENABLE: 0
```

## Connection to real hardware

AVR, ARM Cortex-M, and most MCUs map peripherals into the address space. Datasheets label each register with access permissions — for example, a status register you can only read, or a command register you can only write. Driver code must respect those rules; this emulator practices that constraint in a safe, printable environment.

## Project structure

```
register_emulator/
├── main.c
└── README.md
```
