/**********************************************************************************************************************
 * @file        stm32_device_driver_template.c
 * @brief       Template implementation for STM32 HAL device drivers.
 * @details     Implements initialization, processing, and deinitialization functions for reusable STM32CubeIDE drivers.
 * @author      Satish Kanawade
 * @date        20-May-2026
 * @version     1.0.0
 * @copyright : Satish Kanawade. All rights reserved.
 *********************************************************************************************************************/

#include "stm32_device_driver_template.h"
#include <string.h>

// Validates the driver handle pointer.
static STM32_Driver_Status_t STM32_DeviceDriver_ValidateHandle(const STM32_DeviceDriver_Handle_t *handle)
{
    if (handle == NULL)
    {
        return STM32_DRIVER_INVALID_PARAM;
    }

    if (handle->isInitialized == 0U)
    {
        return STM32_DRIVER_NOT_INITIALIZED;
    }

    return STM32_DRIVER_OK;
}

// Initializes the device driver handle with application-provided configuration.
STM32_Driver_Status_t STM32_DeviceDriver_Init(STM32_DeviceDriver_Handle_t *handle,
                                              const STM32_DeviceDriver_Config_t *config)
{
    if ((handle == NULL) || (config == NULL) || (config->enablePort == NULL))
    {
        return STM32_DRIVER_INVALID_PARAM;
    }

    memset(handle, 0, sizeof(*handle));
    handle->config = *config;
    handle->isInitialized = 1U;

    HAL_GPIO_WritePin(handle->config.enablePort, handle->config.enablePin, GPIO_PIN_SET);

    return STM32_DRIVER_OK;
}

// Runs the driver process function from the application loop or scheduler.
STM32_Driver_Status_t STM32_DeviceDriver_Process(STM32_DeviceDriver_Handle_t *handle)
{
    STM32_Driver_Status_t status = STM32_DeviceDriver_ValidateHandle(handle);
    if (status != STM32_DRIVER_OK)
    {
        return status;
    }

    return STM32_DRIVER_OK;
}

// Deinitializes the device driver handle.
STM32_Driver_Status_t STM32_DeviceDriver_DeInit(STM32_DeviceDriver_Handle_t *handle)
{
    STM32_Driver_Status_t status = STM32_DeviceDriver_ValidateHandle(handle);
    if (status != STM32_DRIVER_OK)
    {
        return status;
    }

    HAL_GPIO_WritePin(handle->config.enablePort, handle->config.enablePin, GPIO_PIN_RESET);
    memset(handle, 0, sizeof(*handle));

    return STM32_DRIVER_OK;
}

