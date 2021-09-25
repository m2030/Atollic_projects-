/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
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

typedef struct {
  uint64_t magic;                                     /**< The USER_CONF_MAGIC magic word signals that the structure was once written to FLASH. */
  char device_name[500];
  char server_name[500];
} iot_config_t;
typedef struct
{
    int time;
    int date;
    float Data;
} DataLogTypeDef;
typedef struct {
  char tls_root_ca_cert[1000]; /* Allow room for 3 root CA certificates */
  char tls_device_cert[1000];
  /* Above structure member must be aligned on 256 byte boundary
     to match firewall constraint , same for the size. */
  char tls_device_key[1000];
#ifdef USE_C2C
  c2c_config_t c2c_config;
#endif
#ifdef USE_WIFI
  wifi_config_t wifi_config;
#endif
  iot_config_t iot_config;
  uint64_t ca_tls_magic;        /**< The USER_CONF_MAGIC magic word signals that the TLS root CA certificates strings
                                    (tls_root_ca_cert) are present in Flash. */
  uint64_t device_tls_magic;    /**< The USER_CONF_MAGIC magic word signals that the TLS device certificate and key
                                    (tls_device_cert and tls_device_key) are present in Flash. */
} user_config_t;
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
