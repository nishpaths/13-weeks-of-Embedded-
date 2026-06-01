# Debugging Lessons

Common mistakes to make on purpose, observe, and fix. That is how the intuition sticks.

## Lesson 1: Assignment vs OR-assign

**Bug:**
```c
PORTA = (1 << 3);
```

**Symptom:** Only bit 3 is set. Every other pin you configured earlier is gone.

**Fix:**
```c
PORTA |= (1 << 3);
```

**Rule:** Never use plain `=` on a shared register unless you intentionally want to replace the entire byte.

---

## Lesson 2: Wrong clear mask

**Bug:**
```c
PORTA &= (1 << 3);
```

**Symptom:** Register becomes `00001000` no matter what it was before. You wanted to clear bit 3 but you kept *only* bit 3.

**Fix:**
```c
PORTA &= ~(1 << 3);
```

**Rule:** To clear, AND with the **inverted** mask. `~` flips every bit in the mask so bit 3 becomes 0 and all others become 1 (pass-through).

---

## Lesson 3: Off-by-one bit number

**Bug:** You wanted to control the 4th LED (counting from 1) and used `(1 << 4)`.

**Symptom:** Wrong LED toggles.

**Fix:** Bit numbers start at 0. The 4th LED is bit 3: `(1 << 3)`.

**Rule:** Hardware docs say "bit 3" or "pin 3" — always confirm whether they mean 0-indexed or 1-indexed.

---

## Lesson 4: Forgetting direction register

**Bug:** You set `PORTA |= (1 << 3)` but the LED does nothing.

**Symptom:** Output latch is set but pin is still configured as input.

**Fix:** Set direction first:
```c
DDRA |= (1 << 3);   /* pin 3 is now output */
PORTA |= (1 << 3);  /* now drive it high */
```

**Rule:** On real hardware, `PORTA` alone is not enough. `DDRA` must be configured first.

---

## Lesson 5: Checking the wrong register

**Bug:**
```c
if (PORTA & (1 << 2))  /* reading output latch */
```

**Symptom:** Switch state does not match reality.

**Fix:** Read input from `PINA`:
```c
if (PINA & (1 << 2))   /* reading actual pin level */
```

**Rule:** For inputs, read `PINx`. For outputs you configured, read `PORTx`.

---

## Debugging discipline

1. **Print in binary** — decimal hides bit patterns. Always use `SHOW` or print `%08b`-style output.
2. **Change one thing at a time** — set one bit, verify, then move on.
3. **Draw the 8 bits** — before and after every operation.
4. **Name your intent** — comment `/* set LED3 */` not `/* shift 1 left 3 */`. Intent-first thinking prevents bugs.
