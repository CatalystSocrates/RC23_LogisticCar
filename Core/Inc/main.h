/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "st7735.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "KF.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "TCS34725.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern float pitch,roll,yaw; 		    
extern short aacx,aacy,aacz;			
extern short gyrox,gyroy,gyroz;		
extern float temp;
extern float Aacx,Aacy,Aacz,Gyrox,Gyroy;
extern int pwma, pwmb,pwmc, pwmd;

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
#define TCS34725_SCL_Pin GPIO_PIN_14
#define TCS34725_SCL_GPIO_Port GPIOC
#define TCS34725_SDA_Pin GPIO_PIN_15
#define TCS34725_SDA_GPIO_Port GPIOC
#define BIN1_Pin GPIO_PIN_0
#define BIN1_GPIO_Port GPIOC
#define BIN2_Pin GPIO_PIN_1
#define BIN2_GPIO_Port GPIOC
#define CIN1_Pin GPIO_PIN_2
#define CIN1_GPIO_Port GPIOC
#define CIN2_Pin GPIO_PIN_3
#define CIN2_GPIO_Port GPIOC
#define PWME_Pin GPIO_PIN_0
#define PWME_GPIO_Port GPIOA
#define PWMF_Pin GPIO_PIN_1
#define PWMF_GPIO_Port GPIOA
#define PWMG_Pin GPIO_PIN_2
#define PWMG_GPIO_Port GPIOA
#define PWMH_Pin GPIO_PIN_3
#define PWMH_GPIO_Port GPIOA
#define EncoderA1_Pin GPIO_PIN_6
#define EncoderA1_GPIO_Port GPIOA
#define EncoderA2_Pin GPIO_PIN_7
#define EncoderA2_GPIO_Port GPIOA
#define DIN1_Pin GPIO_PIN_4
#define DIN1_GPIO_Port GPIOC
#define DIN2_Pin GPIO_PIN_5
#define DIN2_GPIO_Port GPIOC
#define AIN1_Pin GPIO_PIN_0
#define AIN1_GPIO_Port GPIOB
#define AIN2_Pin GPIO_PIN_1
#define AIN2_GPIO_Port GPIOB
#define PWMA_Pin GPIO_PIN_9
#define PWMA_GPIO_Port GPIOE
#define PWMB_Pin GPIO_PIN_11
#define PWMB_GPIO_Port GPIOE
#define PWMC_Pin GPIO_PIN_13
#define PWMC_GPIO_Port GPIOE
#define PWMD_Pin GPIO_PIN_14
#define PWMD_GPIO_Port GPIOE
#define SPI2_CS_Pin GPIO_PIN_12
#define SPI2_CS_GPIO_Port GPIOB
#define SPI2_DC_Pin GPIO_PIN_14
#define SPI2_DC_GPIO_Port GPIOB
#define TCS34725_EXIT_Pin GPIO_PIN_9
#define TCS34725_EXIT_GPIO_Port GPIOD
#define MPU6050_SDA_Pin GPIO_PIN_10
#define MPU6050_SDA_GPIO_Port GPIOD
#define MPU6050_SCL_Pin GPIO_PIN_11
#define MPU6050_SCL_GPIO_Port GPIOD
#define EncoderC1_Pin GPIO_PIN_12
#define EncoderC1_GPIO_Port GPIOD
#define EncoderC2_Pin GPIO_PIN_13
#define EncoderC2_GPIO_Port GPIOD
#define EncoderB1_Pin GPIO_PIN_6
#define EncoderB1_GPIO_Port GPIOC
#define EncoderB2_Pin GPIO_PIN_7
#define EncoderB2_GPIO_Port GPIOC
#define MPU6050_EXIT_Pin GPIO_PIN_8
#define MPU6050_EXIT_GPIO_Port GPIOA
#define SPI2_RST_Pin GPIO_PIN_10
#define SPI2_RST_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
