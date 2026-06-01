# Virtual Register Simulator

A console program that simulates how embedded firmware manipulates microcontroller registers — one bit at a time.

## Why this exists

On a microcontroller, a single byte like `PORTA` controls eight hardware lines. You rarely assign the whole byte. Instead you **target individual bits** with masks:

```c
PORTA |= (1 << 3);   /* turn ON  bit 3, preserve everything else */
PORTA &= ~(1 << 5);  /* turn OFF bit 5, preserve everything else */
```

This simulator trains that mental model before you touch real hardware.

## Why masks exist

A mask is a pattern of bits that selects *which* positions you care about.

`(1 << 3)` produces `00001000` — only bit 3 is 1. Every other bit is 0.

When you OR that mask into a register, only bit 3 can change to 1. All other bits stay whatever they were. That is **controlled mutation**: you touch one pin without disturbing the rest.

## Why `|=` preserves state

Assignment replaces the entire register:

```c
PORTA = (1 << 3);  /* WRONG for "turn on bit 3" — wipes bits 0,1,2,4,5,6,7 */
```

Compound OR merges the mask into the existing value:

```c
PORTA |= (1 << 3); /* CORRECT — bit 3 becomes 1, all other bits unchanged */
```

Embedded firmware almost always uses `|=`, `&=`, and `^=` because registers are shared state. One function might control the LED on bit 3 while another controls a motor on bit 5. Wiping the register breaks everything else.

## Why `&(1 << n)` checks a bit

AND keeps a 1 only where **both** operands have a 1:

```c
if (PORTA & (1 << 3))  /* is bit 3 set? */
```

If bit 3 is 1, the AND result is non-zero (true). If bit 3 is 0, the result is zero (false). No other bits affect the answer because the mask zeroes them out.

## Registers in this simulator

| Register | Role |
|----------|------|
| `PORTA`  | Output latch — drives pins high (1) or low (0) |
| `DDRA`   | Data direction — 1 = output, 0 = input |
| `PINA`   | Pin input state — simulated switch readings |

This mirrors AVR architecture (Arduino Uno uses AVR). Same concepts apply to ARM, PIC, and most MCUs.

## Build and run

```bash
gcc src/main.c src/registers.c -o register-simulator
./register-simulator
```

On Windows with MinGW:

```powershell
gcc src/main.c src/registers.c -o register-simulator.exe
.\register-simulator.exe
```

## Commands

```
SET 3              Set bit 3 on PORTA
CLEAR 3            Clear bit 3 on PORTA
TOGGLE 5           Toggle bit 5 on PORTA
CHECK 2            Read bit 2 on PORTA
SET DDRA 3         Set bit 3 on DDRA (any register works)
OUTPUT 3           Shortcut: make pin 3 an output
INPUT 3            Shortcut: make pin 3 an input
SHOW               Display all registers, bit states, and LEDs
HELP               List commands
QUIT               Exit
```

## Example session

```
> SET 3
Set PORTA bit 3
> SET 5
Set PORTA bit 5
> SHOW
PORTA: 00101000
DDRA:  00000000
PINA:  00000000

Bit 0 = OFF
...
Bit 5 = ON
...

LED0 OFF
...
LED5 ON
```

## The final challenge

Look at this line and visualize the result **without running code**:

```c
PORTA &= ~(1 << 5);
```

- Mask: `~(1 << 5)` → `11011111`
- Target: bit 5
- Effect: force bit 5 to 0
- Preserved: bits 0–4 and 6–7 stay unchanged

When you can do that instantly, bitwise operations are internalized.

## Project structure

```
register-simulator/
├── src/
│   ├── main.c
│   ├── registers.c
│   └── registers.h
├── notes/
│   ├── debugging-lessons.md
│   └── bitwise-insights.md
└── README.md
```
