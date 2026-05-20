# STM32 Device Driver Template

This template provides a clean starting point for STM32CubeIDE HAL/LL device drivers.

## Files

| File | Purpose |
| --- | --- |
| `Inc/stm32_device_driver_template.h` | Public API, status codes, configuration structure, and function prototypes. |
| `Src/stm32_device_driver_template.c` | Driver implementation and parameter validation. |

## How To Use

1. Copy this folder and rename it for the target device.
2. Rename the `.c` and `.h` files.
3. Replace `STM32_DeviceDriver` names with the actual driver name.
4. Add HAL handles, GPIO pins, bus addresses, or timing values to the configuration structure.
5. Include the header in your CubeIDE application.
6. Add the source file to the CubeIDE build.

@copyright : Satish Kanawade. All rights reserved.

