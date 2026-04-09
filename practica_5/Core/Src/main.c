/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>

#include "API_cmdparser.h"
#include "API_uart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/**
 * @brief Prints over uart the list of available commands and their description.
 * @param[in] args pointer to the array of arguments received with the command.
 * @param[in] arg_count number of arguments received with the command.
 * @return CMD_OK if the command was executed successfully, CMD_ERR_UNKNOWN if an error occurred while executing the command
 * @note parameters in this function are not used since we don't expect any arguments
 * for the HELP command, but they are required to match the function pointer type
 * in cmd_t struct
 */
static cmd_status_t print_help(const char args[][CMD_MAX_LINE/3], uint8_t arg_count);

/**
 * @brief Turns the LED on.
 * @param[in] args pointer to the array of arguments received with the command.
 * @param[in] arg_count number of arguments received with the command.
 * @return CMD_OK if the command was executed successfully, CMD_ERR_UNKNOWN if an error occurred while executing the command
 * @note parameters in this function are not used since we don't expect any arguments
 * for the LED ON command, but they are required to match the function pointer type
 * in cmd_t struct
 */
static cmd_status_t led_on(const char args[][CMD_MAX_LINE/3], uint8_t arg_count);

/**
 * @brief Turns the LED off.
 * @param[in] args pointer to the array of arguments received with the command.
 * @param[in] arg_count number of arguments received with the command.
 * @return CMD_OK if the command was executed successfully, CMD_ERR_UNKNOWN if an error occurred while executing the command
 * @note parameters in this function are not used since we don't expect any arguments
 * for the LED OFF command, but they are required to match the function pointer type
 */
static cmd_status_t led_off(const char args[][CMD_MAX_LINE/3], uint8_t arg_count);

/**
 * @brief Toggles the LED.
 * @param[in] args pointer to the array of arguments received with the command.
 * @param[in] arg_count number of arguments received with the command.
 * @return CMD_OK if the command was executed successfully, CMD_ERR_UNKNOWN if an error occurred while executing the command
 * @note parameters in this function are not used since we don't expect any arguments
 * for the LED TOGGLE command, but they are required to match the function pointer type
 */
static cmd_status_t led_toggle(const char args[][CMD_MAX_LINE/3], uint8_t arg_count);

/**
 * @brief Returns the current status of the LED.
 * @param[in] args pointer to the array of arguments received with the command.
 * @param[in] arg_count number of arguments received with the command.
 * @return CMD_OK if the command was executed successfully, CMD_ERR_UNKNOWN if an error occurred while executing the command
 * @note parameters in this function are not used since we don't expect any arguments
 * for the STATUS command, but they are required to match the function pointer type
 */
static cmd_status_t led_status(const char args[][CMD_MAX_LINE/3], uint8_t arg_count);

/**
 * @brief Gets the current baudrate.
 * @param[in] args pointer to the array of arguments received with the command.
 * @param[in] arg_count number of arguments received with the command.
 * @return CMD_OK if the command was executed successfully, CMD_ERR_UNKNOWN if an error occurred while executing the command
 * @note parameters in this function are not used since we don't expect any arguments
 * for the BAUD? command, but they are required to match the function pointer type
 */
static cmd_status_t get_baudrate(const char args[][CMD_MAX_LINE/3], uint8_t arg_count);

/**
 * @brief Sets the baudrate.
 * @param[in] args pointer to the array of arguments received with the command.
 * @param[in] arg_count number of arguments received with the command.
 * @return CMD_OK if the command was executed successfully, CMD_ERR_UNKNOWN if an error occurred while executing the command
 */
static cmd_status_t set_baudrate(const char args[][CMD_MAX_LINE/3], uint8_t arg_count);

/**
 * @brief Data structure containing a list of cmd_t useful for quickly matching received commands with their corresponding function and description for help text.
 *
 * This allow us to quickly add a new command without much boilerplate code
 */
static cmd_t cmds[] = {
		{.cmd = "LED ON", .desc = "\t\t\tTurns the led on", .func = led_on},
		{.cmd = "LED OFF", .desc = "\t\t\tTurns the led off", .func = led_off},
		{.cmd = "LED TOGGLE", .desc = "\t\tToggles the led", .func = led_toggle},
		{.cmd = "STATUS", .desc = "\t\t\tReturns current LED status", .func = led_status},
		{.cmd = "BAUD", .desc = "=<BAUDRATE>\t\tsets baudrate and restart console. Valid baudrates are between 9600 and 921600", .func = set_baudrate},
		{.cmd = "BAUD?", .desc = "\t\t\tPrints current baudrate", .func = get_baudrate},
		{.cmd = "HELP", .desc = "\t\t\tPrints available commands", .func = print_help },
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  if(uartInit() != UART_OK)
  {
	  Error_Handler();
  }
  cmdParserInit(cmds, sizeof(cmds)/sizeof(cmds[0]));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  cmdPoll();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static cmd_status_t set_baudrate(const char args[][CMD_MAX_LINE/3], uint8_t arg_counter)
{
	char *endptr;
	// we expect only one argument
	if(arg_counter != 1) {
		return CMD_ERR_ARG;
	}
	
	// parse baudrate value from argument
	uint32_t new_baudrate = (uint32_t)strtoul(args[0], &endptr, 10);
	// check if we found digits, if not return CMD_ERR_ARG
	if(args[1] == endptr) {
		return CMD_ERR_ARG;
	}
	// set baudrate
	uart_status_t uart_ret = uartSetBaudrate(new_baudrate);
	if(uart_ret == UART_EINVAL) {
		return CMD_ERR_ARG;
	} else if(uart_ret != UART_OK) {
		return CMD_ERR_UNKNOWN;
	}
	return CMD_OK;
}

static cmd_status_t get_baudrate(const char [][CMD_MAX_LINE/3], uint8_t)
{
	uint32_t curr_baudrate = uartGetBaudrate();
	char tmp[MAX_TX_SIZE];
	snprintf(tmp, sizeof(tmp), "Current baudrate is %lu\r\n", curr_baudrate);
	uartSendString(tmp);
	return CMD_OK;
}

static cmd_status_t print_help(const char [][CMD_MAX_LINE/3], uint8_t)
{
	uartSendString("Available commands:\r\n");
	uartSendString("\r\n");
	size_t i;
	for(i = 0; i < sizeof(cmds)/sizeof(cmds[0]); i++) {
		uartSendString(cmds[i].cmd);
		uartSendString(cmds[i].desc);
		uartSendString("\r\n");
	}
	return CMD_OK;
}

static cmd_status_t led_on(const char [][CMD_MAX_LINE/3], uint8_t)
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	return CMD_OK;
}

static cmd_status_t led_off(const char [][CMD_MAX_LINE/3], uint8_t)
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	return CMD_OK;
}

static cmd_status_t led_toggle(const char [][CMD_MAX_LINE/3], uint8_t)
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	return CMD_OK;
}

static cmd_status_t led_status(const char [][CMD_MAX_LINE/3], uint8_t)
{
	GPIO_PinState state = HAL_GPIO_ReadPin(LD2_GPIO_Port, LD2_Pin);
	char tmp[MAX_TX_SIZE];
	snprintf(tmp, sizeof(tmp), "LED is %s\r\n", (state == GPIO_PIN_SET) ? "ON" : "OFF");
	uartSendString(tmp);
	return CMD_OK;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
