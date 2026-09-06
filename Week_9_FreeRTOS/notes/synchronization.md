# Synchronization notes

## Queues

- **Queue** = thread-safe mailbox for data (here: `sensor_sample_t`).
- Producer `put`, consumer `get` (with timeout or `osWaitForever`).
- Two queues (display + log) so one slow consumer does not block the other from receiving.

## Mutex (mutual exclusion)

- UART is a shared peripheral. Without a mutex, two tasks can interleave bytes mid-line.
- Pattern: `acquire` → transmit → `release`.
- Do not hold a mutex across long `osDelay` calls.

## Semaphores (concept)

- **Binary semaphore:** signal “ISR or task said go” (often for deferred interrupt work).
- **Counting semaphore:** limit N concurrent users of a resource pool.
- This week uses a **mutex** (priority-inheritance capable in FreeRTOS) for UART ownership — prefer mutex over binary semaphore when protecting a resource.

## Blocking vs starvation

| Behavior | Cause |
|----------|--------|
| Blocking | Task waits on queue/mutex/delay — OK and expected |
| Starvation | Low-priority task never runs because higher tasks spin without blocking |
| Fix | Ensure hot tasks call `osDelay` or block on IPC |

## Debugging practice

1. Comment out `osMutexAcquire/Release` — watch UART garble.
2. Stop `DisplayTask` from reading — watch `qDisplay` fill (add a counter of failed puts).
3. Raise Heartbeat to Realtime and busy-loop — starve everyone (then undo).
4. Compare `HAL_Delay(200)` vs `osDelay(200)` in SensorTask under a debugger.
