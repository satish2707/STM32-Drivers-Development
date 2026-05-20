/**********************************************************************************************************************
 * @file        stm32_device_driver_template.h
 * @brief       Template interface for STM32 HAL device drivers.
 * @details     Provides a reusable driver structure with configuration, status values, initialization, processing,
 *              and deinitialization APIs for STM32CubeIDE projects.
 * @author      Satish Kanawade
 * @date        20-May-2026
 * @version     1.0.0
 * @copyright : Satish Kanawade. All rights reserved.
 *********************************************************************************************************************/

#ifndef STM32_DEVICE_DRIVER_TEMPLATE_H_
#define STM32_DEVICE_DRIVER_TEMPLATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef enum
{
    STM32_DRIVER_OK = 0,
    STM32_DRIVER_ERROR,
    STM32_DRIVER_INVALID_PARAM,
    STM32_DRIVER_NOT_INITIALIZED
} STM32_Driver_Status_t;

typedef struct
{
    GPIO_TypeDef *enablePort;
    uint16_t enablePin;
    uint32_t timeoutMs;
} STM32_DeviceDriver_Config_t;

typedef struct
{
    STM32_DeviceDriver_Config_t config;
    uint8_t isInitialized;
} STM32_DeviceDriver_Handle_t;

/**
 * @brief Initializes the device driver handle with application-provided configuration.
 * @param handle Pointer to the driver handle.
 * @param config Pointer to the driver configuration.
 * @return Driver operation status.
 */
STM32_Driver_Status_t STM32_DeviceDriver_Init(STM32_DeviceDriver_Handle_t *handle,
                                              const STM32_DeviceDriver_Config_t *config);

/**
 * @brief Runs the driver process function from the application loop or scheduler.
 * @param handle Pointer to the initialized driver handle.
 * @return Driver operation status.
 */
STM32_Driver_Status_t STM32_DeviceDriver_Process(STM32_DeviceDriver_Handle_t *handle);

/**
 * @brief Deinitializes the device driver handle.
 * @param handle Pointer to the initialized driver handle.
 * @return Driver operation status.
 */
STM32_Driver_Status_t STM32_DeviceDriver_DeInit(STM32_DeviceDriver_Handle_t *handle);

#ifdef __cplusplus
}
#endif

#endif

