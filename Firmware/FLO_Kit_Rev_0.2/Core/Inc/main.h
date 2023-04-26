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
#include "stm32f0xx_hal.h"

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
#define BOOT_MODE_Pin GPIO_PIN_4
#define BOOT_MODE_GPIO_Port GPIOA
#define BOOT_BUTTON_Pin GPIO_PIN_6
#define BOOT_BUTTON_GPIO_Port GPIOA
#define RED_LED_Pin GPIO_PIN_0
#define RED_LED_GPIO_Port GPIOB
#define GREEN_LED_Pin GPIO_PIN_1
#define GREEN_LED_GPIO_Port GPIOB
#define BLUE_LED_Pin GPIO_PIN_2
#define BLUE_LED_GPIO_Port GPIOB
#define FAN_PWM_Pin GPIO_PIN_12
#define FAN_PWM_GPIO_Port GPIOA
#define HAPTICS_FB_Pin GPIO_PIN_15
#define HAPTICS_FB_GPIO_Port GPIOA
#define VOLUME_UP_Pin GPIO_PIN_3
#define VOLUME_UP_GPIO_Port GPIOB
#define VOLUME_DOWN_Pin GPIO_PIN_4
#define VOLUME_DOWN_GPIO_Port GPIOB
#define POWER_Pin GPIO_PIN_5
#define POWER_GPIO_Port GPIOB
#define DEBUG_LED_Pin GPIO_PIN_8
#define DEBUG_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
