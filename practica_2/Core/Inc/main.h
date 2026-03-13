/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef uint32_t tick_t;
typedef bool bool_t;

/**
 * @brief estructura utilizada para generar un retardo no bloqueante
 */
typedef struct {
  tick_t startTime; /// Tick inicial, utilizado como referencia para calcular la diferencia temporal
  tick_t duration; /// Duracion en ticks en el cual el retardo llega a su fin
  bool_t running; /// bandera para determinar si el delay esta corriend o no
} delay_t;

/**
 * @brief Inicializa el objeto delay
 *
 * @param delay objeto delay_t a inicializar
 * @param duration duracion del timer
 * @note: no pone a correr el delay, solo inicializa la variable duration
 */
void delayInit(delay_t * delay, tick_t duration);

/**
 * @brief Lee el delay para determinar si llego a su fin o no
 *
 * @param delay objeto delay_t a inicializar
 * @return devuelve true si el delay llego a su fin, si no, significa que no termino.
 * @note si el delay _no_ esta corriendo, pone el delay a correr.
 */
bool_t delayRead(delay_t * delay);

/**
 * @brief Modifica el valor duration del objeto delay
 *
 * @param delay objeto delay_t a inicializar
 * @param duration duracion del timer
 */
void delayWrite(delay_t* delay, tick_t duration);
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
