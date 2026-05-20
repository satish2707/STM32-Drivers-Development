# Driver Development Guide

## New Driver Checklist

1. Create `drivers/<driver_name>/Inc` and `drivers/<driver_name>/Src`.
2. Add a public header and source file.
3. Define configuration, handle, and status types.
4. Add `Init`, `Process` or service, and `DeInit` APIs where applicable.
5. Keep STM32CubeMX generated code outside the reusable driver.
6. Add a driver README with wiring, CubeMX setup, API details, and example usage.
7. Test the driver on hardware and record the tested board/MCU.

## Recommended Public API

```c
Driver_Status_t Driver_Init(Driver_Handle_t *handle, const Driver_Config_t *config);
Driver_Status_t Driver_Process(Driver_Handle_t *handle);
Driver_Status_t Driver_DeInit(Driver_Handle_t *handle);
```

## Documentation Required Per Driver

- Overview
- Hardware requirements
- CubeMX peripheral configuration
- Public API table
- Example `main.c` integration
- Tested board or MCU

@copyright : Satish Kanawade. All rights reserved.

