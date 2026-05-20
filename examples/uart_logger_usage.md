# STM32 UART Logger Usage Example

This example shows how to use `drivers/uart_logger` in a normal STM32CubeIDE project.

## Target

- Any STM32 MCU with one UART/USART peripheral
- STM32CubeIDE generated HAL project
- USB-to-serial adapter or ST-LINK virtual COM port

## CubeMX Setup

1. Enable `USART2` or any available UART.
2. Select `Asynchronous` mode.
3. Set baud rate to `115200`.
4. Keep data format as `8-N-1`.
5. Generate code.

## Connection

| STM32 Pin | USB-to-Serial Adapter |
| --- | --- |
| UART TX | RX |
| UART RX | TX, optional for future command input |
| GND | GND |

## main.c Integration

```c
#include "main.h"
#include "usart.h"
#include "stm32_uart_logger.h"

static STM32_UART_Logger_Handle_t uartLogger;

static void App_LoggerInit(void)
{
    STM32_UART_Logger_Config_t config = {
        .huart = &huart2,
        .txTimeoutMs = 100U,
        .minimumLevel = STM32_UART_LOGGER_LEVEL_DEBUG,
        .enableTimestamp = 1U,
        .enableLevelPrefix = 1U,
        .lineEnding = "\r\n"
    };

    (void)STM32_UART_Logger_Init(&uartLogger, &config);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    App_LoggerInit();

    STM32_LOG_INFO(&uartLogger, "STM32 UART logger ready");
    STM32_LOG_DEBUG(&uartLogger, "Build date: %s %s", __DATE__, __TIME__);

    while (1)
    {
        STM32_LOG_INFO(&uartLogger, "Heartbeat tick: %lu", (unsigned long)HAL_GetTick());
        HAL_Delay(1000U);
    }
}
```

## Expected Serial Output

```text
[12 ms] [INF] STM32 UART logger ready
[13 ms] [DBG] Build date: May 20 2026 10:19:00
[1015 ms] [INF] Heartbeat tick: 1015
```

@copyright : Satish Kanawade. All rights reserved.
