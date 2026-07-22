# Day 4 — UART Communication

## Goal

Send and receive text between the STM32 and a computer using UART.

By the end of the session, the STM32 could:

- send strings to the PC
- send individual bytes
- receive bytes from the PC
- control an LED using commands
- respond with confirmation messages
- use a small reusable UART module

---

## What is UART?

UART stands for **Universal Asynchronous Receiver/Transmitter**.

UART is a serial communication method that allows two devices to exchange data one bit at a time.

Examples:

- STM32 to PC
- PC to STM32
- STM32 to another microcontroller
- STM32 to a sensor or communication module

UART is **asynchronous**, which means there is no shared clock wire. Both devices must agree on the communication settings beforehand.

---

## TX and RX

**TX** means Transmit — the line used to send data out of a device.

**RX** means Receive — the line used to receive data into a device.

Connections between two UART devices are crossed:

```
Device A TX → Device B RX
Device A RX ← Device B TX
```

One device's output must connect to the other device's input.

---

## USART versus UART

The STM32 peripheral is named USART2.

USART stands for **Universal Synchronous/Asynchronous Receiver/Transmitter**.

When USART2 is configured in Asynchronous mode, it is being used as UART.

```
USART2 hardware peripheral
        ↓
Configured as Asynchronous
        ↓
Used as UART
```

For the Nucleo-F446RE setup:

| Pin | Function    |
|-----|-------------|
| PA2 | USART2_TX   |
| PA3 | USART2_RX   |

---

## UART configuration

The UART was configured as:

| Setting      | Value  |
|--------------|--------|
| Baud rate    | 115200 |
| Data bits    | 8      |
| Parity       | None   |
| Stop bits    | 1      |
| Flow control | None   |

This is often written as:

```
115200, 8-N-1
```

Meaning:

- `115200` baud
- `8` data bits
- `N` = no parity
- `1` stop bit

---

## Baud rate

Baud rate controls how fast data is transmitted.

For basic UART, it is approximately the number of transmitted bits or symbols per second.

Both devices must use matching baud rates.

Example:

| Device | Baud rate |
|--------|-----------|
| STM32  | 115200    |
| PC     | 115200    |

If the STM32 transmits at 115200 but the PC listens at 9600, the receiver samples the signal at the wrong times.

Possible symptoms:

- garbage characters
- unreadable text
- missing characters
- corrupted output

---

## What is a COM port?

A COM port is the name Windows gives to a serial connection.

Example:

```
COM5 - STMicroelectronics STLink Virtual COM Port
```

The COM port tells the serial terminal which connected serial device to open.

```
UART settings = how to communicate
COM port      = which device to communicate with
```

The communication path used was:

```
STM32 USART2
    ↓
PA2 TX / PA3 RX
    ↓
ST-Link virtual COM bridge
    ↓
USB cable
    ↓
Windows COM5
    ↓
VS Code Serial Monitor
```

The ST-Link acts as a bridge between UART signals and USB.

---

## Serial terminal

The serial terminal used was the VS Code Serial Monitor.

| Setting    | Value  |
|------------|--------|
| Port       | COM5   |
| Baud rate  | 115200 |
| Data bits  | 8      |
| Parity     | None   |
| Stop bits  | 1      |
| View mode  | Text   |

For one-character command testing, the terminal line ending was set to:

```
None
```

---

## First UART transmission

The first successful message was:

```
STM32 UART working
```

This proved the complete transmit path worked:

```
main.c
→ HAL UART transmission
→ USART2
→ ST-Link
→ COM5
→ VS Code Serial Monitor
```

A simple startup message is useful because it creates a known-good communication test.

---

## Why send a counter?

A counter proves the STM32 can repeatedly send changing runtime information.

Example output:

```
Counter: 0
Counter: 1
Counter: 2
```

The counter itself is not important. It demonstrates the same pattern later used for:

- `Brightness: 50%`
- `Traffic state: RED`
- `ADC value: 2048`
- `Temperature: 24 C`

UART allows the board to report what is happening internally.

Without UART, the STM32 can feel like a black box.

---

## Strings, bytes, and ASCII

The serial terminal sends text as character bytes.

For example, `'1'` is the character 1. Its ASCII value is `49`.

This is different from raw numeric byte value `1`.

Therefore:

```c
received_byte == '1'   // correct — checks what the user typed
received_byte == 1     // wrong — raw byte value 1
```

Another example:

| Value | Meaning              |
|-------|----------------------|
| `'8'` | ASCII 56             |
| `8`   | raw byte value 8     |

A raw byte value such as 8 may be a non-printable control character, so the terminal may display nothing.

---

## Echo test

The echo test worked like this:

```
PC sends a character
→ STM32 receives it
→ STM32 sends the same byte back
→ terminal displays it
```

Example:

```
PC sends: 8
STM32 echoes: 8
```

This proved both directions worked:

- PC → STM32
- STM32 → PC

---

## Newline characters

Two common newline characters are:

- `\r` = carriage return
- `\n` = line feed / newline

Together, `\r\n` is called **CRLF**.

When the serial terminal sent `0` with CRLF, the STM32 actually received three separate bytes:

```
'0'
'\r'
'\n'
```

The MCU receives one byte at a time, so `\r` and `\n` can appear as extra commands.

For the one-byte command test, setting the serial monitor line ending to **None** fixed this.

Later, a command parser can intentionally ignore `\r` and `\n`.

---

## Blocking UART

The blocking receive function waits until:

- a byte arrives, or
- the timeout expires

During that wait, the normal `while (1)` application logic does not continue.

Code after the receive call cannot run until the call returns.

Examples of main-loop work that may stop updating:

- LED fade logic
- button polling
- state-machine updates
- periodic debug messages
- other software tasks

Hardware interrupts may still run, but normal main-loop code is stalled.

This is why interrupt-based UART reception is useful later.

---

## HAL status values

The UART receive function returns a status.

| Status        | Meaning                                              |
|---------------|------------------------------------------------------|
| `HAL_OK`      | A byte was received successfully                     |
| `HAL_TIMEOUT` | No byte arrived before the timeout expired           |
| `HAL_ERROR`   | An actual UART error occurred                        |
| `HAL_BUSY`    | The UART peripheral or HAL operation is currently busy |

`HAL_TIMEOUT` does not automatically mean the UART is broken.

The received byte and the status are separate pieces of information:

```
status        → Did reception succeed?
received_byte → What data was received?
```

A timeout should not be treated like an unknown command.

---

## Command test

The working command behavior was:

```
Send '1'
→ LED turns on
→ STM32 sends "LED ON"

Send '0'
→ LED turns off
→ STM32 sends "LED OFF"
```

Correct logic:

```
Receive one byte

If status is HAL_OK:
    If byte is '0':
        turn LED off
        send confirmation

    Else if byte is '1':
        turn LED on
        send confirmation

    Else:
        optionally report unknown command

If status is HAL_TIMEOUT:
    do nothing and try again
```

---

## UART module

The UART abstraction was split into:

- `uart.h`
- `uart.c`

The module supports:

- `uart_init`
- `uart_send_byte`
- `uart_send_string`
- `uart_receive_byte`

The module stores:

- a pointer to the HAL UART handle
- the timeout value

The timeout uses `uint32_t` instead of `uint8_t`.

A `uint8_t` can only store values from 0 to 255, so it cannot represent values such as:

- `500` ms
- `1000` ms
- `HAL_MAX_DELAY`

### What the UART module hides

The UART module hides HAL details from `main.c`, including:

- `HAL_UART_Transmit`
- `HAL_UART_Receive`
- the UART handle
- the byte count
- timeout handling
- string length handling

This allows `main.c` to think in higher-level actions:

- send string
- send byte
- receive byte
- process command

Instead of repeatedly dealing with raw HAL calls.

---

## Important bugs and lessons

### Bug: Missing closing brace

Compiler errors included:

- invalid storage class for function
- expected declaration or statement at end of input

**Cause:** `main()` or the while loop was not properly closed before generated functions began.

**Lesson:** Compiler errors near generated functions can be caused by a syntax error earlier in the file.

### Bug: Sending numeric 8

Code used:

```c
uint8_t received_byte = 8;
```

The terminal displayed nothing.

**Cause:** Raw byte value 8 is not the printable character `'8'`.

**Fix:**

```c
uint8_t received_byte = '8';
```

### Bug: Comparing against 0 and 1

Incorrect:

```c
received_byte == 0
received_byte == 1
```

Correct:

```c
received_byte == '0'
received_byte == '1'
```

**Cause:** The serial terminal sends ASCII character bytes.

### Bug: Ignoring receive status

The program checked `received_byte` without first checking whether reception succeeded.

This can create stale-data behavior after a timeout.

**Fix:** Only inspect the byte when `status == HAL_OK`.

### Bug: Treating timeout as an error

The program printed `Error receiving byte` every time no new byte arrived within one second.

**Cause:** `HAL_TIMEOUT` was handled as though it were `HAL_ERROR`.

**Fix:** Ignore `HAL_TIMEOUT` for this simple polling test.

### Bug: Extra commands from CRLF

The terminal sent `0` with CRLF. The MCU received:

```
'0'
'\r'
'\n'
```

**Fix:** Set line ending to None for one-byte commands, or ignore newline characters in software.

### Bug: Returning from `main()`

Returning from `main()` after a receive timeout would stop the firmware application.

On an embedded system, `main()` normally runs forever.

**Fix:** Do not return from `main()` for normal UART timeouts.

---

## Final understanding

UART allows the STM32 and another device to exchange bytes through TX and RX.

Both devices must agree on settings such as baud rate, data bits, parity, and stop bits.

The PC uses a COM port to identify which serial device the terminal should open.

The current receive implementation is blocking because the main loop waits for a byte or timeout before continuing.

The UART module makes the application cleaner by hiding HAL-specific details.
