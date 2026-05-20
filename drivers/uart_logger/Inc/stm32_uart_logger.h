/**********************************************************************************************************************
 * @file        stm32_uart_logger.h
 * @brief       Reusable UART logger interface for STM32 HAL projects.
 * @details     Provides formatted log output, log levels, optional timestamps, and raw UART write helpers for
 *              STM32CubeIDE projects using any STM32 HAL series.
 * @author      Satish Kanawade
 * @date        20-May-2026
 * @version     1.0.0
 * @copyright : Satish Kanawade. All rights reserved.
 *********************************************************************************************************************/

#ifndef STM32_UART_LOGGER_H_
#define STM32_UART_LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#ifndef STM32_UART_LOGGER_HAL_INCLUDE
#define STM32_UART_LOGGER_HAL_INCLUDE "main.h"
#endif

#include STM32_UART_LOGGER_HAL_INCLUDE

#ifndef STM32_UART_LOGGER_BUFFER_SIZE
#define STM32_UART_LOGGER_BUFFER_SIZE (256U)
#endif

#if (STM32_UART_LOGGER_BUFFER_SIZE < 32U)
#error "STM32_UART_LOGGER_BUFFER_SIZE must be at least 32 bytes."
#endif

#ifndef STM32_UART_LOGGER_DEFAULT_TIMEOUT_MS
#define STM32_UART_LOGGER_DEFAULT_TIMEOUT_MS (100U)
#endif

#define STM32_UART_LOGGER_LEVEL_VALUE_NONE  (0U)
#define STM32_UART_LOGGER_LEVEL_VALUE_ERROR (1U)
#define STM32_UART_LOGGER_LEVEL_VALUE_WARN  (2U)
#define STM32_UART_LOGGER_LEVEL_VALUE_INFO  (3U)
#define STM32_UART_LOGGER_LEVEL_VALUE_DEBUG (4U)

#ifndef STM32_UART_LOGGER_COMPILE_LEVEL
#define STM32_UART_LOGGER_COMPILE_LEVEL STM32_UART_LOGGER_LEVEL_VALUE_DEBUG
#endif

typedef enum
{
    STM32_UART_LOGGER_OK = 0,
    STM32_UART_LOGGER_ERROR,
    STM32_UART_LOGGER_INVALID_PARAM,
    STM32_UART_LOGGER_NOT_INITIALIZED,
    STM32_UART_LOGGER_BUFFER_OVERFLOW
} STM32_UART_Logger_Status_t;

typedef enum
{
    STM32_UART_LOGGER_LEVEL_NONE = STM32_UART_LOGGER_LEVEL_VALUE_NONE,
    STM32_UART_LOGGER_LEVEL_ERROR = STM32_UART_LOGGER_LEVEL_VALUE_ERROR,
    STM32_UART_LOGGER_LEVEL_WARN = STM32_UART_LOGGER_LEVEL_VALUE_WARN,
    STM32_UART_LOGGER_LEVEL_INFO = STM32_UART_LOGGER_LEVEL_VALUE_INFO,
    STM32_UART_LOGGER_LEVEL_DEBUG = STM32_UART_LOGGER_LEVEL_VALUE_DEBUG
} STM32_UART_Logger_Level_t;

typedef struct
{
    UART_HandleTypeDef *huart;
    uint32_t txTimeoutMs;
    STM32_UART_Logger_Level_t minimumLevel;
    uint8_t enableTimestamp;
    uint8_t enableLevelPrefix;
    const char *lineEnding;
} STM32_UART_Logger_Config_t;

typedef struct
{
    STM32_UART_Logger_Config_t config;
    uint8_t isInitialized;
} STM32_UART_Logger_Handle_t;

/**
 * @brief Initializes the UART logger handle with application-provided UART configuration.
 * @param handle Pointer to the logger handle.
 * @param config Pointer to the logger configuration.
 * @return Logger operation status.
 */
STM32_UART_Logger_Status_t STM32_UART_Logger_Init(STM32_UART_Logger_Handle_t *handle,
                                                  const STM32_UART_Logger_Config_t *config);

/**
 * @brief Deinitializes the UART logger handle.
 * @param handle Pointer to the initialized logger handle.
 * @return Logger operation status.
 */
STM32_UART_Logger_Status_t STM32_UART_Logger_DeInit(STM32_UART_Logger_Handle_t *handle);

/**
 * @brief Updates the runtime minimum log level.
 * @param handle Pointer to the initialized logger handle.
 * @param level Minimum log level to transmit.
 * @return Logger operation status.
 */
STM32_UART_Logger_Status_t STM32_UART_Logger_SetLevel(STM32_UART_Logger_Handle_t *handle,
                                                      STM32_UART_Logger_Level_t level);

/**
 * @brief Writes raw bytes to the configured UART.
 * @param handle Pointer to the initialized logger handle.
 * @param data Pointer to bytes that will be transmitted.
 * @param length Number of bytes to transmit.
 * @return Logger operation status.
 */
STM32_UART_Logger_Status_t STM32_UART_Logger_Write(STM32_UART_Logger_Handle_t *handle,
                                                   const uint8_t *data,
                                                   size_t length);

/**
 * @brief Writes a null-terminated string to the configured UART.
 * @param handle Pointer to the initialized logger handle.
 * @param text Null-terminated string to transmit.
 * @return Logger operation status.
 */
STM32_UART_Logger_Status_t STM32_UART_Logger_WriteString(STM32_UART_Logger_Handle_t *handle,
                                                         const char *text);

/**
 * @brief Writes a null-terminated string followed by the configured line ending.
 * @param handle Pointer to the initialized logger handle.
 * @param text Null-terminated string to transmit.
 * @return Logger operation status.
 */
STM32_UART_Logger_Status_t STM32_UART_Logger_WriteLine(STM32_UART_Logger_Handle_t *handle,
                                                       const char *text);

/**
 * @brief Formats and transmits one log line using the selected log level.
 * @param handle Pointer to the initialized logger handle.
 * @param level Log level used for filtering and prefix text.
 * @param format printf-style format string.
 * @return Logger operation status.
 */
STM32_UART_Logger_Status_t STM32_UART_Logger_Log(STM32_UART_Logger_Handle_t *handle,
                                                 STM32_UART_Logger_Level_t level,
                                                 const char *format,
                                                 ...);

/**
 * @brief Returns whether the logger handle is initialized.
 * @param handle Pointer to the logger handle.
 * @return 1 when initialized, otherwise 0.
 */
uint8_t STM32_UART_Logger_IsInitialized(const STM32_UART_Logger_Handle_t *handle);

/**
 * @brief Returns the millisecond tick value used in timestamped log lines.
 * @return Millisecond tick count.
 */
uint32_t STM32_UART_Logger_GetTick(void);

#if (STM32_UART_LOGGER_COMPILE_LEVEL >= STM32_UART_LOGGER_LEVEL_VALUE_ERROR)
#define STM32_LOG_ERROR(handle, format, ...) \
    ((void)STM32_UART_Logger_Log((handle), STM32_UART_LOGGER_LEVEL_ERROR, (format), ##__VA_ARGS__))
#else
#define STM32_LOG_ERROR(handle, format, ...) ((void)0)
#endif

#if (STM32_UART_LOGGER_COMPILE_LEVEL >= STM32_UART_LOGGER_LEVEL_VALUE_WARN)
#define STM32_LOG_WARN(handle, format, ...) \
    ((void)STM32_UART_Logger_Log((handle), STM32_UART_LOGGER_LEVEL_WARN, (format), ##__VA_ARGS__))
#else
#define STM32_LOG_WARN(handle, format, ...) ((void)0)
#endif

#if (STM32_UART_LOGGER_COMPILE_LEVEL >= STM32_UART_LOGGER_LEVEL_VALUE_INFO)
#define STM32_LOG_INFO(handle, format, ...) \
    ((void)STM32_UART_Logger_Log((handle), STM32_UART_LOGGER_LEVEL_INFO, (format), ##__VA_ARGS__))
#else
#define STM32_LOG_INFO(handle, format, ...) ((void)0)
#endif

#if (STM32_UART_LOGGER_COMPILE_LEVEL >= STM32_UART_LOGGER_LEVEL_VALUE_DEBUG)
#define STM32_LOG_DEBUG(handle, format, ...) \
    ((void)STM32_UART_Logger_Log((handle), STM32_UART_LOGGER_LEVEL_DEBUG, (format), ##__VA_ARGS__))
#else
#define STM32_LOG_DEBUG(handle, format, ...) ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif
