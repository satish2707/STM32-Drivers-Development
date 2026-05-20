/**********************************************************************************************************************
 * @file        stm32_uart_logger.c
 * @brief       Reusable UART logger implementation for STM32 HAL projects.
 * @details     Implements blocking UART log transmission, formatted messages, log level filtering, optional
 *              timestamps, and raw write helpers without dynamic memory allocation.
 * @author      Satish Kanawade
 * @date        20-May-2026
 * @version     1.0.0
 * @copyright : Satish Kanawade. All rights reserved.
 *********************************************************************************************************************/

#include "stm32_uart_logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifndef STM32_UART_LOGGER_WEAK
#if defined(__GNUC__)
#define STM32_UART_LOGGER_WEAK __attribute__((weak))
#else
#define STM32_UART_LOGGER_WEAK __weak
#endif
#endif

#define STM32_UART_LOGGER_MAX_UART_CHUNK (0xFFFFU)

// Returns the configured line ending or the default CRLF ending.
static const char *STM32_UART_Logger_GetLineEnding(const STM32_UART_Logger_Handle_t *handle)
{
    if ((handle->config.lineEnding == NULL) || (handle->config.lineEnding[0] == '\0'))
    {
        return "\r\n";
    }

    return handle->config.lineEnding;
}

// Returns the printable text prefix for a log level.
static const char *STM32_UART_Logger_GetLevelText(STM32_UART_Logger_Level_t level)
{
    switch (level)
    {
        case STM32_UART_LOGGER_LEVEL_ERROR:
            return "ERR";

        case STM32_UART_LOGGER_LEVEL_WARN:
            return "WRN";

        case STM32_UART_LOGGER_LEVEL_INFO:
            return "INF";

        case STM32_UART_LOGGER_LEVEL_DEBUG:
            return "DBG";

        default:
            return "";
    }
}

// Validates a log level value.
static STM32_UART_Logger_Status_t STM32_UART_Logger_ValidateLevel(STM32_UART_Logger_Level_t level)
{
    if (level > STM32_UART_LOGGER_LEVEL_DEBUG)
    {
        return STM32_UART_LOGGER_INVALID_PARAM;
    }

    return STM32_UART_LOGGER_OK;
}

// Validates an initialized logger handle.
static STM32_UART_Logger_Status_t STM32_UART_Logger_ValidateHandle(const STM32_UART_Logger_Handle_t *handle)
{
    if (handle == NULL)
    {
        return STM32_UART_LOGGER_INVALID_PARAM;
    }

    if (handle->isInitialized == 0U)
    {
        return STM32_UART_LOGGER_NOT_INITIALIZED;
    }

    if (handle->config.huart == NULL)
    {
        return STM32_UART_LOGGER_INVALID_PARAM;
    }

    return STM32_UART_LOGGER_OK;
}

// Appends formatted text to the local logger buffer.
static STM32_UART_Logger_Status_t STM32_UART_Logger_AppendFormat(char *buffer,
                                                                 size_t bufferSize,
                                                                 size_t *offset,
                                                                 const char *format,
                                                                 ...)
{
    int written;
    va_list args;

    if ((buffer == NULL) || (offset == NULL) || (format == NULL) || (*offset >= bufferSize))
    {
        return STM32_UART_LOGGER_INVALID_PARAM;
    }

    va_start(args, format);
    written = vsnprintf(&buffer[*offset], bufferSize - *offset, format, args);
    va_end(args);

    if (written < 0)
    {
        return STM32_UART_LOGGER_ERROR;
    }

    if ((size_t)written >= (bufferSize - *offset))
    {
        *offset = bufferSize - 1U;
        return STM32_UART_LOGGER_BUFFER_OVERFLOW;
    }

    *offset += (size_t)written;
    return STM32_UART_LOGGER_OK;
}

// Appends the configured line ending to the local logger buffer.
static STM32_UART_Logger_Status_t STM32_UART_Logger_AppendLineEnding(STM32_UART_Logger_Handle_t *handle,
                                                                     char *buffer,
                                                                     size_t bufferSize,
                                                                     size_t *offset)
{
    const char *lineEnding = STM32_UART_Logger_GetLineEnding(handle);
    size_t lineEndingLength = strlen(lineEnding);
    size_t availableLength;

    if ((buffer == NULL) || (offset == NULL) || (*offset >= bufferSize))
    {
        return STM32_UART_LOGGER_INVALID_PARAM;
    }

    availableLength = bufferSize - *offset - 1U;
    if (lineEndingLength > availableLength)
    {
        memcpy(&buffer[*offset], lineEnding, availableLength);
        *offset += availableLength;
        buffer[*offset] = '\0';
        return STM32_UART_LOGGER_BUFFER_OVERFLOW;
    }

    memcpy(&buffer[*offset], lineEnding, lineEndingLength);
    *offset += lineEndingLength;
    buffer[*offset] = '\0';

    return STM32_UART_LOGGER_OK;
}

// Initializes the UART logger handle with application-provided UART configuration.
STM32_UART_Logger_Status_t STM32_UART_Logger_Init(STM32_UART_Logger_Handle_t *handle,
                                                  const STM32_UART_Logger_Config_t *config)
{
    if ((handle == NULL) || (config == NULL) || (config->huart == NULL))
    {
        return STM32_UART_LOGGER_INVALID_PARAM;
    }

    if (STM32_UART_Logger_ValidateLevel(config->minimumLevel) != STM32_UART_LOGGER_OK)
    {
        return STM32_UART_LOGGER_INVALID_PARAM;
    }

    memset(handle, 0, sizeof(*handle));
    handle->config = *config;

    if (handle->config.txTimeoutMs == 0U)
    {
        handle->config.txTimeoutMs = STM32_UART_LOGGER_DEFAULT_TIMEOUT_MS;
    }

    if (handle->config.lineEnding == NULL)
    {
        handle->config.lineEnding = "\r\n";
    }

    handle->isInitialized = 1U;

    return STM32_UART_LOGGER_OK;
}

// Deinitializes the UART logger handle.
STM32_UART_Logger_Status_t STM32_UART_Logger_DeInit(STM32_UART_Logger_Handle_t *handle)
{
    STM32_UART_Logger_Status_t status = STM32_UART_Logger_ValidateHandle(handle);

    if (status != STM32_UART_LOGGER_OK)
    {
        return status;
    }

    memset(handle, 0, sizeof(*handle));
    return STM32_UART_LOGGER_OK;
}

// Updates the runtime minimum log level.
STM32_UART_Logger_Status_t STM32_UART_Logger_SetLevel(STM32_UART_Logger_Handle_t *handle,
                                                      STM32_UART_Logger_Level_t level)
{
    STM32_UART_Logger_Status_t status = STM32_UART_Logger_ValidateHandle(handle);

    if (status != STM32_UART_LOGGER_OK)
    {
        return status;
    }

    if (STM32_UART_Logger_ValidateLevel(level) != STM32_UART_LOGGER_OK)
    {
        return STM32_UART_LOGGER_INVALID_PARAM;
    }

    handle->config.minimumLevel = level;
    return STM32_UART_LOGGER_OK;
}

// Writes raw bytes to the configured UART.
STM32_UART_Logger_Status_t STM32_UART_Logger_Write(STM32_UART_Logger_Handle_t *handle,
                                                   const uint8_t *data,
                                                   size_t length)
{
    STM32_UART_Logger_Status_t status = STM32_UART_Logger_ValidateHandle(handle);
    const uint8_t *cursor = data;
    size_t remainingLength = length;

    if (status != STM32_UART_LOGGER_OK)
    {
        return status;
    }

    if ((data == NULL) && (length > 0U))
    {
        return STM32_UART_LOGGER_INVALID_PARAM;
    }

    while (remainingLength > 0U)
    {
        uint16_t chunkLength = (remainingLength > STM32_UART_LOGGER_MAX_UART_CHUNK)
                                   ? (uint16_t)STM32_UART_LOGGER_MAX_UART_CHUNK
                                   : (uint16_t)remainingLength;

        if (HAL_UART_Transmit(handle->config.huart, (uint8_t *)cursor, chunkLength, handle->config.txTimeoutMs) != HAL_OK)
        {
            return STM32_UART_LOGGER_ERROR;
        }

        cursor += chunkLength;
        remainingLength -= chunkLength;
    }

    return STM32_UART_LOGGER_OK;
}

// Writes a null-terminated string to the configured UART.
STM32_UART_Logger_Status_t STM32_UART_Logger_WriteString(STM32_UART_Logger_Handle_t *handle,
                                                         const char *text)
{
    if (text == NULL)
    {
        return STM32_UART_LOGGER_INVALID_PARAM;
    }

    return STM32_UART_Logger_Write(handle, (const uint8_t *)text, strlen(text));
}

// Writes a null-terminated string followed by the configured line ending.
STM32_UART_Logger_Status_t STM32_UART_Logger_WriteLine(STM32_UART_Logger_Handle_t *handle,
                                                       const char *text)
{
    STM32_UART_Logger_Status_t status;

    if (text == NULL)
    {
        return STM32_UART_LOGGER_INVALID_PARAM;
    }

    status = STM32_UART_Logger_WriteString(handle, text);
    if (status != STM32_UART_LOGGER_OK)
    {
        return status;
    }

    return STM32_UART_Logger_WriteString(handle, STM32_UART_Logger_GetLineEnding(handle));
}

// Formats and transmits one log line using the selected log level.
STM32_UART_Logger_Status_t STM32_UART_Logger_Log(STM32_UART_Logger_Handle_t *handle,
                                                 STM32_UART_Logger_Level_t level,
                                                 const char *format,
                                                 ...)
{
    STM32_UART_Logger_Status_t status = STM32_UART_Logger_ValidateHandle(handle);
    STM32_UART_Logger_Status_t formatStatus = STM32_UART_LOGGER_OK;
    char buffer[STM32_UART_LOGGER_BUFFER_SIZE];
    size_t offset = 0U;
    va_list args;
    int written;

    if (status != STM32_UART_LOGGER_OK)
    {
        return status;
    }

    if ((format == NULL) || (STM32_UART_Logger_ValidateLevel(level) != STM32_UART_LOGGER_OK))
    {
        return STM32_UART_LOGGER_INVALID_PARAM;
    }

    if ((level == STM32_UART_LOGGER_LEVEL_NONE) || (level > handle->config.minimumLevel))
    {
        return STM32_UART_LOGGER_OK;
    }

    buffer[0] = '\0';

    if (handle->config.enableTimestamp != 0U)
    {
        formatStatus = STM32_UART_Logger_AppendFormat(buffer,
                                                      sizeof(buffer),
                                                      &offset,
                                                      "[%lu ms] ",
                                                      (unsigned long)STM32_UART_Logger_GetTick());
    }

    if ((formatStatus == STM32_UART_LOGGER_OK) && (handle->config.enableLevelPrefix != 0U))
    {
        formatStatus = STM32_UART_Logger_AppendFormat(buffer,
                                                      sizeof(buffer),
                                                      &offset,
                                                      "[%s] ",
                                                      STM32_UART_Logger_GetLevelText(level));
    }

    if (formatStatus == STM32_UART_LOGGER_OK)
    {
        va_start(args, format);
        written = vsnprintf(&buffer[offset], sizeof(buffer) - offset, format, args);
        va_end(args);

        if (written < 0)
        {
            return STM32_UART_LOGGER_ERROR;
        }

        if ((size_t)written >= (sizeof(buffer) - offset))
        {
            offset = sizeof(buffer) - 1U;
            formatStatus = STM32_UART_LOGGER_BUFFER_OVERFLOW;
        }
        else
        {
            offset += (size_t)written;
        }
    }

    if ((formatStatus == STM32_UART_LOGGER_OK) || (formatStatus == STM32_UART_LOGGER_BUFFER_OVERFLOW))
    {
        STM32_UART_Logger_Status_t lineStatus = STM32_UART_Logger_AppendLineEnding(handle, buffer, sizeof(buffer), &offset);
        if ((formatStatus == STM32_UART_LOGGER_OK) && (lineStatus != STM32_UART_LOGGER_OK))
        {
            formatStatus = lineStatus;
        }
    }

    status = STM32_UART_Logger_Write(handle, (const uint8_t *)buffer, offset);
    if (status != STM32_UART_LOGGER_OK)
    {
        return status;
    }

    return formatStatus;
}

// Returns whether the logger handle is initialized.
uint8_t STM32_UART_Logger_IsInitialized(const STM32_UART_Logger_Handle_t *handle)
{
    if (handle == NULL)
    {
        return 0U;
    }

    return handle->isInitialized;
}

// Returns the millisecond tick value used in timestamped log lines.
STM32_UART_LOGGER_WEAK uint32_t STM32_UART_Logger_GetTick(void)
{
    return HAL_GetTick();
}
