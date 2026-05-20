# STM32CubeIDE Integration

## Add A Driver To CubeIDE

1. Copy the driver `Inc` folder path into the CubeIDE include path.
2. Add the driver `Src/*.c` files to the project source tree.
3. Include the driver header in `main.c` or your application module.
4. Configure GPIO, I2C, SPI, UART, timer, DMA, or interrupt settings in STM32CubeMX.
5. Initialize the driver after `MX_GPIO_Init()` and required peripheral init functions.
6. Call process or service functions from `while (1)` or from your scheduler.

## Suggested CubeIDE Folder Mapping

```text
Core/
  Inc/
  Src/
Drivers_User/
  Inc/
  Src/
```

## Notes

- Keep `.ioc` files committed for demo projects.
- Do not commit `Debug/` or `Release/` build outputs.
- Keep board-specific setup in the CubeIDE project and reusable logic in `drivers/`.

@copyright : Satish Kanawade. All rights reserved.

