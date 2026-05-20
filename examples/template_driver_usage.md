# Template Driver Usage

Example `main.c` integration pattern:

```c
#include "stm32_device_driver_template.h"

static STM32_DeviceDriver_Handle_t deviceDriver;

void App_Init(void)
{
    STM32_DeviceDriver_Config_t config = {
        .enablePort = GPIOA,
        .enablePin = GPIO_PIN_5,
        .timeoutMs = 1000U,
    };

    (void)STM32_DeviceDriver_Init(&deviceDriver, &config);
}

void App_Loop(void)
{
    (void)STM32_DeviceDriver_Process(&deviceDriver);
}
```

@copyright : Satish Kanawade. All rights reserved.

