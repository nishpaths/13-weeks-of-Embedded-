# Bitwise Insights

Notes from building the register simulator. Read these when an operation feels like syntax instead of meaning.

## `1 << n` is always bit *n*, not bit *n+1*

Bits are numbered from 0 (rightmost) to 7 (leftmost):

```
Bit:  7 6 5 4 3 2 1 0
      0 0 0 0 1 0 0 0   ← this is (1 << 3), NOT bit 4
```

Off-by-one here is one of the most common embedded bugs.

## Three ways to set a bit — only one is correct for shared registers

```c
PORTA = (1 << 3);    /* assignment: DESTROYS all other bits */
PORTA |= (1 << 3);   /* OR-assign: sets bit 3, preserves rest  ✓ */
PORTA = PORTA | (1 << 3);  /* same as |=, just longer */
```

## Three ways to clear a bit

```c
PORTA = 0;              /* nuclear — clears everything */
PORTA &= (1 << 3);      /* BUG — keeps ONLY bit 3, zeros rest */
PORTA &= ~(1 << 3);     /* correct — clears bit 3, preserves rest ✓ */
```

The `&= (1 << 3)` mistake is subtle. Beginners think "I'm operating on bit 3" but AND with that mask **keeps only bit 3** and clears everything else.

## Toggle with XOR

```c
PORTA ^= (1 << 3);
```

XOR flips bits where the mask is 1. If bit 3 was 0 it becomes 1; if it was 1 it becomes 0. Other bits are unchanged because `0 ^ 0 = 0`.

## Reading vs writing registers

| Goal | Pattern |
|------|---------|
| Set bit | `reg \|= (1 << n)` |
| Clear bit | `reg &= ~(1 << n)` |
| Toggle bit | `reg ^= (1 << n)` |
| Test bit | `reg & (1 << n)` |

The read (test) operation never modifies the register. The write operations always use compound assignment (`|=`, `&=`, `^=`) to preserve unrelated bits.

## Why `uint8_t` matters

Registers are fixed-width. An `int` might be 32 bits on your PC but the hardware register is 8 bits. Using `uint8_t` forces you to think in terms of exactly 8 bits — matching the silicon.

## Mask visualization exercise

For any operation, write out the 8-bit row:

```
Before:  0 0 1 0 1 0 0 0
Mask:    0 0 0 1 0 0 0 0   (1 << 3)
After:   0 0 1 1 1 0 0 0   (OR — bit 3 joins the 1s)
```

Do this on paper until it is automatic.
