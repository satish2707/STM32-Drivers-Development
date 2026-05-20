# STM32 Drivers Development

Dedicated STM32CubeIDE device driver development repository for STM32 HAL/LL based embedded projects. This repo is prepared for reusable STM32 device drivers, hardware interface modules, CubeIDE integration notes, and example usage documentation.

## Purpose

This repository is intended for STM32 device driver work using STM32CubeIDE, STM32CubeMX generated projects, and the STM32 HAL or LL libraries. Keep reusable driver logic here, then copy or link the driver `Inc` and `Src` files into application projects.

## Repository Structure

| Path | Purpose |
| --- | --- |
| `drivers/` | Reusable STM32 device driver modules. |
| `drivers/template/` | Starter `.c/.h` driver template with professional headers. |
| `examples/` | Example usage notes and code snippets for drivers. |
| `projects/` | Optional STM32CubeIDE project notes or board-specific demo projects. |
| `docs/` | Coding standard, driver guide, and CubeIDE integration notes. |

## Driver Folder Pattern

Use this structure for every new driver:

```text
drivers/<driver_name>/
  Inc/
    stm32_<driver_name>.h
  Src/
    stm32_<driver_name>.c
  README.md
```

## Quick Start

1. Create a new STM32CubeIDE project or open an existing CubeIDE project.
2. Add the required driver header from `drivers/<driver_name>/Inc`.
3. Add the required driver source from `drivers/<driver_name>/Src`.
4. Include the driver header in your application file, usually `main.c`.
5. Pass required HAL handles, GPIO ports, pins, or peripheral configuration values into the driver init function.
6. Build and test on the target STM32 board.

## Development Rules

- Use STM32 HAL or LL APIs consistently inside each driver.
- Keep hardware-specific pins and handles configurable from the application.
- Keep driver code reusable and independent from one fixed CubeIDE project.
- Add one README per driver with hardware wiring, dependencies, API usage, and example code.
- Add professional file headers to every `.c` and `.h` file.
- Use this copyright line in all source files:

```text
@copyright : Satish Kanawade. All rights reserved.
```

## Current Status

Initial STM32 CubeIDE driver-development structure is ready. Add real STM32 device drivers inside `drivers/` using the provided template.

