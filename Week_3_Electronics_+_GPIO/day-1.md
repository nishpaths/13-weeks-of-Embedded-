First STM32 blink test:
- Board: NUCLEO-F446RE
- LED: LD2
- Pin: PA5
- Peripheral: GPIOA
- CubeMX config: PA5 as GPIO_Output
- Code used: HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin)
- Delay test: changed HAL_Delay values and confirmed blink speed changed
- Debug issue: ST-LINK not detected after update; fixed by unplug/replug board

