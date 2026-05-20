# Coding Standard

## File Headers

Every `.c` and `.h` file should include:

```c
/**********************************************************************************************************************
 * @file        file_name.c
 * @brief       Short file purpose.
 * @details     Detailed file purpose.
 * @author      Satish Kanawade
 * @date        DD-MMM-YYYY
 * @version     1.0.0
 * @copyright : Satish Kanawade. All rights reserved.
 *********************************************************************************************************************/
```

## Function Comments

Use a short one-line comment before every function implementation:

```c
// Initializes the device driver.
Driver_Status_t Driver_Init(Driver_Handle_t *handle)
{
}
```

## Naming

- Public functions: `STM32_DriverName_FunctionName`
- Private functions: `static` and clear module-local names
- Types: `DriverName_Handle_t`, `DriverName_Config_t`, `DriverName_Status_t`
- Macros: uppercase with module prefix

## Driver Design

- Do not hardcode STM32 pins inside reusable drivers unless documented.
- Pass HAL handles, GPIO ports, GPIO pins, addresses, and timing values through config structures.
- Return status codes instead of silently failing.
- Keep blocking delays small and documented.
- Keep CubeIDE generated code separate from reusable driver code.

@copyright : Satish Kanawade. All rights reserved.

