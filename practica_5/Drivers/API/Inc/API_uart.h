#ifndef API_UART_H_
#define API_UART_H_

#include "API_delay.h"

#define MAX_TX_SIZE 256
#define MIN_TX_SIZE 1
#define MAX_RX_SIZE 256
#define MIN_RX_SIZE 1
#define MIN_BAUDRATE 9600

/**
 * @brief representation of uart operation status
 */
typedef enum {
	UART_OK = 0,
	UART_ERROR,
	UART_EINVAL,
} uart_status_t;

/**
 * @brief representation of supported baudrates
*/
typedef enum {
	BR_INVALID = 0,
	BR_9600 = 9600,
	BR_115200 = 115200,
	BR_38400 = 38400,
	BR_57600 = 57600,
	BR_230400 = 230400,
	BR_460800 = 460800,
	BR_921600 = 921600,
} uart_baudrate_t;

/**
 * @brief initialize UART instance
 * @return uart_status_t value indicating the result of the operation
 * @note it will print over UART a msg with it's configuration parameters
 */
uart_status_t uartInit();

/**
 * @brief sends a string over uart
 * @param[in] pString: pointer to array of chars
 */
void uartSendString(char *pstring);

/**
 * @brief send a string over uart with a specific size
 * @param[in] pString: pointer to array of chars
 * @param[in] size: size of string. Must be between 1 and 256
 */
void uartSendStringSize(char *pString, uint16_t size);

/**
 * @brief receive a string over uart
 * @param[out] pstring: pointer to buffer where the data will be stored
 * @param[in] size: amount of bytes to read
 * @return uart_status_t value indicating the result of the operation
 */
uart_status_t uartReceiveStringSize(char *pstring, uint16_t size);

/**
 * @brief returns the current configured baudrateA
 * @return uint32_t value of current baudrate
 */
uint32_t uartGetBaudrate();

/**
 * @brief set baudrate
 *
 * Sets the current interface baudrate and restats it
 * @param[in] uint32_t value of baudrate to set
 * @return uart_status_t value indicating the result of the operation
 */
uart_status_t uartSetBaudrate(uint32_t baudrate);

#endif /* API_UART_H_ */
