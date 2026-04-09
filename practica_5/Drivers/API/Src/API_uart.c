#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "API_uart.h"
#include "stm32f4xx_hal.h"

#define DEFAULT_BAUDRATE 115200U

static UART_HandleTypeDef huart;
static uint8_t tx_buffer[MAX_TX_SIZE];

/**
 * @brief helper function to initialize UART with the specified baudrate
 * @param[in] baudrate baudrate to initialize UART with
 * @return HAL_OK if initialization was successful, HAL_ERROR otherwise
 */
static HAL_StatusTypeDef _uartInit(uint32_t baudrate)
{
  huart.Instance = USART2;
  huart.Init.BaudRate = baudrate;
  huart.Init.WordLength = UART_WORDLENGTH_8B;
  huart.Init.StopBits = UART_STOPBITS_1;
  huart.Init.Parity = UART_PARITY_NONE;
  huart.Init.Mode = UART_MODE_TX_RX;
  huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart.Init.OverSampling = UART_OVERSAMPLING_16;
  return HAL_UART_Init(&huart);
}

/**
 * @brief helper function to validate if the provided baudrate is supported and
 * return the closest one
 * @param[in] baudrate baudrate to validate
 * @return uart_baudrate_t value of the closest supported baudrate,
 * BR_INVALID if the provided baudrate is out of range
 */
static uart_baudrate_t validateBaudrate(uint32_t baudrate)
{
	const uart_baudrate_t supported_baudrates[] = {BR_9600, BR_115200, BR_38400, BR_57600, BR_230400, BR_460800, BR_921600};
	size_t i;
	for(i = 0; i < sizeof(supported_baudrates)/sizeof(supported_baudrates[0]); i++) {
		if(baudrate < supported_baudrates[i + 1]) {
			if(baudrate - supported_baudrates[i] < supported_baudrates[i + 1] - baudrate) {
				return supported_baudrates[i];
			} else {
				return supported_baudrates[i + 1];
			}
		}
	}
	return BR_INVALID;
}

uart_status_t uartInit()
{
  if(_uartInit(DEFAULT_BAUDRATE) != HAL_OK)
  {
	  return UART_ERROR;
  }
  snprintf((char*)tx_buffer,
		   sizeof(tx_buffer)/sizeof(tx_buffer[0]),
		   "baudrate: %lu, word length: %u, stop bits: %u\r\n", // parity: %s, mode: %lu, hw flow ctrl: %lu, oversampling: %lu\r\n",
		   huart.Init.BaudRate, (huart.Init.WordLength == 0) ? 8 : 9,
		   (huart.Init.StopBits == 0) ? 1 : 2); // (huart.Init.Parity == UART_PARITY_NONE) "none" ? "even", huart.Init.Mode,
		   //huart.Init.HwFlowCtl, huart.Init.OverSampling);
  uartSendString((char*)tx_buffer);
  return UART_OK;
}

uint32_t uartGetBaudrate()
{
	return huart.Init.BaudRate;
}

uart_status_t uartSetBaudrate(uint32_t baudrate)
{
	uart_baudrate_t valid_baudrate = validateBaudrate(baudrate);
	if(valid_baudrate == BR_INVALID) {
		return UART_EINVAL;
	}
	if(HAL_UART_DeInit(&huart) != HAL_OK)
	{
		return UART_ERROR;
	}
	huart.Init.BaudRate = baudrate;
	if(_uartInit(baudrate) != HAL_OK)
	{
		return UART_ERROR;
	}
	return UART_OK;
}

void uartSendString(char *pstring)
{
	if(pstring == NULL) {
		return;
	}
	size_t size = strlen((char*)pstring);
	if(size > MAX_TX_SIZE || size < MIN_TX_SIZE)
	{
		return;
	}
	HAL_UART_Transmit(&huart, (uint8_t*) pstring, (uint16_t)size, HAL_MAX_DELAY);
}

void uartSendStringSize(char *pstring, uint16_t size)
{
	if(pstring == NULL)
	{
		return;
	}
	if(size > MAX_TX_SIZE || size < MIN_TX_SIZE)
	{
		return;
	}
	HAL_UART_Transmit(&huart, (uint8_t*) pstring, size, HAL_MAX_DELAY);
}

uart_status_t uartReceiveStringSize(char *pstring, uint16_t size)
{
	if(pstring == NULL)
	{
		return UART_EINVAL;
	}
	if(size > MAX_RX_SIZE || size < MIN_RX_SIZE)
	{
		return UART_EINVAL;
	}
	return HAL_UART_Receive(&huart, (uint8_t*) pstring, size, 0) == HAL_OK;
}
