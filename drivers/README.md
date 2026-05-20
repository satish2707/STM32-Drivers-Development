# Drivers

Reusable STM32 device drivers live in this directory.

## Add A New Driver

1. Copy `drivers/template`.
2. Rename the copied folder to your driver name, for example `ds3231_rtc` or `w5500_ethernet`.
3. Rename the template `.c` and `.h` files.
4. Update the file header, API names, configuration structure, and README.
5. Add usage notes and CubeIDE integration steps.

## Recommended Driver Layout

```text
drivers/<driver_name>/
  Inc/
  Src/
  README.md
```

@copyright : Satish Kanawade. All rights reserved.

