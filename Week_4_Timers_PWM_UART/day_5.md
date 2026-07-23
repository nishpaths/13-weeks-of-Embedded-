# Day 5 — UART Receive Interrupts and Command Interface

## Goal

Receive UART input without blocking the rest of the program.

Build a small command interface that can receive complete text commands and control an LED.

---

## Commands Implemented

- `led on`
  - Turns the LED on.

- `led off`
  - Turns the LED off.

- `status`
  - Reports whether the LED is currently on or off.

- `help`
  - Displays the available commands.

---

## Program Architecture

The program receives UART data one byte at a time.

Flow:

1. `HAL_UART_Receive_IT()` starts a one-byte receive operation.
2. A byte arrives through USART2.
3. The USART2 interrupt occurs.
4. `USART2_IRQHandler()` calls the HAL UART interrupt handler.
5. HAL calls `HAL_UART_RxCpltCallback()`.
6. The callback stores the byte in the command buffer.
7. When `\r` or `\n` is received, the command is null-terminated.
8. `command_ready` becomes true.
9. The main loop processes the completed command.
10. UART reception is re-armed for the next byte.

---

## Important Variables

### `received_byte`

What does this variable store?

- it stores one character of the command that the PC sends over. 

Why can it not store an entire command such as `led on`?

- because received byte is only one byte wide 

---

### `command_buffer`

What does the command buffer store?

- it stores the whole command in a character array (AKA a string)

Why is a buffer required for multi-character commands?

- a buffer is required because UART can't do multi character comamnds it does it one byte per interrupt request. 

---

### `command_index`

What does `command_index` represent?

- it represents next position where a received character will be stored

Why does it increase every time a normal character is received?

- it increases after each stored character so the next character is written into the next unused location instead of overwriting the previous one.

---

### `command_ready`

What does this flag mean when it becomes true?

- a complete command has been received and is ready for the main loop to process/do. 

Why is it declared `volatile`?

- because the interrupt callback can change it asynchronously while the main loop is reading it. 

---

## Blocking vs Interrupt-Based Reception

### Blocking receive

Example function:

`HAL_UART_Receive()`

Why can blocking reception be a problem?

- Blocking reception makes the CPU wait inside HAL_UART_Receive() until data arives or the timeout expires. 

What happens to the main loop while the function waits for input?

- the main loop stops progressing 

---

### Interrupt-based receive

Example function:

`HAL_UART_Receive_IT()`

Why is interrupt-based reception better for this project?

- it allows the UART hardware to wait for data while the CPU continues executing tasks in the main loop. When a byte arrives, the interrup briefly handles it and then normal execution continues. 

What can the main loop continue doing while waiting for UART input?

- While waiting for the UART input the main loop can do anything that's not related to that UART input (e.g: toggling a seperate LED)

---

## UART Receive Callback

What causes `HAL_UART_RxCpltCallback()` to run?

- an interrupt request

What work does the callback perform?

- it makes sure that the command string is passed through so command_ready = true.

What work should not happen inside the callback?

- The behavior or what the comammnd should do, shouldn't happen inside the callback. 

Why should command parsing happen in the main loop instead?

- Parsing and executing commands may take longer than the minimal receive work. Keeping that logic outside the interrupt allows the callback to finish quickly. 

---

## Re-Arming Reception

Why must this operation be called again after receiving a byte?

`HAL_UART_Receive_IT(...)`

- Each call to HAL_UART_Receive_IT() starts one receive operation for a specified number of bytes. Since this project requests one byte, the operation ends after one byte arrives. It must be called again to receive another byte. 

What happened when reception was not restarted?

- The program received the first byte, ran the callback once, and then stopped receiving additional bytes.

How many bytes were received before reception stopped?

- 1 byte

---

## Command Buffer Operation

When the command `led on` is entered, the bytes arrive approximately like this:

```text
'l'
'e'
'd'
' '
'o'
'n'
'\r'
'\n'