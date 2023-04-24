/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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
#define Buzzer_Pin GPIO_PIN_3
#define Buzzer_GPIO_Port GPIOA
#define Key1_Pin GPIO_PIN_4
#define Key1_GPIO_Port GPIOA
#define Key2_Pin GPIO_PIN_5
#define Key2_GPIO_Port GPIOA
#define Key3_Pin GPIO_PIN_6
#define Key3_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_11
#define LED2_GPIO_Port GPIOB
#define ROM_SDA_Pin GPIO_PIN_12
#define ROM_SDA_GPIO_Port GPIOB
#define ROM_SCL_Pin GPIO_PIN_13
#define ROM_SCL_GPIO_Port GPIOB
#define Trig_Pin GPIO_PIN_11
#define Trig_GPIO_Port GPIOA
#define Echo_Pin GPIO_PIN_12
#define Echo_GPIO_Port GPIOA
#define DS18B20_Pin GPIO_PIN_15
#define DS18B20_GPIO_Port GPIOA
#define DS1302_SCLK_Pin GPIO_PIN_3
#define DS1302_SCLK_GPIO_Port GPIOB
#define DS1302_IO_Pin GPIO_PIN_4
#define DS1302_IO_GPIO_Port GPIOB
#define DS1302_CE_Pin GPIO_PIN_5
#define DS1302_CE_GPIO_Port GPIOB
#define ROTARY_S2_Pin GPIO_PIN_6
#define ROTARY_S2_GPIO_Port GPIOB
#define ROTARY_S2_EXTI_IRQn EXTI9_5_IRQn
#define ROTARY_S1_Pin GPIO_PIN_7
#define ROTARY_S1_GPIO_Port GPIOB
#define ROTARY_S1_EXTI_IRQn EXTI9_5_IRQn
#define I2C_SDA_Pin GPIO_PIN_8
#define I2C_SDA_GPIO_Port GPIOB
#define I2C_SCL_Pin GPIO_PIN_9
#define I2C_SCL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
