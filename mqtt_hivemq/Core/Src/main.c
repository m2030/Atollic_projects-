/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lwip/apps/http_client.h"
#include <stdbool.h>
#include "lwip/apps/mqtt.h"
mqtt_client_t *client;

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

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
char buf[5000];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void fetc(void);
void rec(void);
char pacote[1000];

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void adjust_RTC(char *p);
void HttpClientResultCallback (void *arg, httpc_result_t httpc_result, u32_t
rx_content_len, u32_t srv_res, err_t err) ;
err_t RecvHttpHeaderCallback (httpc_state_t *connection, void *arg, struct
pbuf *hdr, u16_t hdr_len, u32_t content_len);
uint8_t timerequestflag=0;
char timerbuffer[275];
uint8_t aShowTime[50] = {0};
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};
uint8_t aShowDate[50] = {0};
static void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate);
#if (defined(__GNUC__) && !defined(__CC_ARM))
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART2 and Loop until the end of transmission */
  while (HAL_OK != HAL_UART_Transmit(&huart3, (uint8_t *) &ch, 1, 30000))
  {
    ;
  }
  return ch;
}

/**
  * @brief  Retargets the C library scanf function to the USART.
  * @param  None
  * @retval None
  */
GETCHAR_PROTOTYPE
{
  /* Place your implementation of fgetc here */
  /* e.g. read a character on USART and loop until the end of read */
  uint8_t ch = 0;
  while (HAL_OK != HAL_UART_Receive(&huart3, (uint8_t *)&ch, 1, 30000))
  {
    ;
  }
  return ch;
}
uint16_t years_int, Month_int, hours_int, mintues_int, days_int, second_int;

ip4_addr_t resolved;
static void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  sprintf((char *)showtime, "time %d:%d:%d\n\r", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  HAL_UART_Transmit(&huart3,showtime,strlen(showtime),1000);
  /* Display date Format : mm-dd-yy */
  sprintf((char *)showdate, "date %d-%d-%d\n\r", sdatestructureget.Date, sdatestructureget.Month,sdatestructureget.Year);
	HAL_UART_Transmit(&huart3,showdate,strlen(showdate),1000);

}
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
  MX_RTC_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_LWIP_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  client = mqtt_client_new();

  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
 		  {
 		    /* Initialization Error */
 		    Error_Handler();
 		  }

 		  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
 		  /* Start Channel1 */
 		  if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
 		  {
 		    /* Starting Error */
 		    Error_Handler();
 		  }
//httpd_init();
 fetc();

 //while(timerequestflag!=1){}
//printf("achived\n\r");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  MX_LWIP_Process();
    /* USER CODE END WHILE */

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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 216;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 2000000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
err_t RecvpayloadCallback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p,
		err_t err){
	HAL_GPIO_TogglePin(LD1_GPIO_Port,LD1_Pin);
	sprintf(buf,"RecvpayloadCallback\n");
	 HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	 if (p == NULL) {sprintf(buf,"pbuf==Null\n");
	 HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	 }
	 else{
		 strncpy(timerbuffer, p->payload, 270);
		 sprintf(buf,"timerbuffer :%s\n\r",timerbuffer);
	HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	 timerequestflag=1;
	 rec();
	 }

	 pbuf_free(p);
	 return ERR_OK;
}
err_t RecvHttpHeaderCallback (httpc_state_t *connection, void *arg, struct
pbuf *hdr, u16_t hdr_len, u32_t content_len) {
	HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
   sprintf(buf,"RecvHttpHeaderCallback error");
	HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
   return ERR_OK;
}
void HttpClientResultCallback (void *arg, httpc_result_t httpc_result, u32_t
rx_content_len, u32_t srv_res, err_t err) {
	HAL_GPIO_TogglePin(LD3_GPIO_Port,LD3_Pin);
	sprintf(buf,"HttpClientResultCallback \n\r");
	HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	sprintf(buf,"httpc_result: %u\n", httpc_result);
	HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	sprintf(buf,"received number of bytes: %lu\n", rx_content_len);

}
void rec(void){
	volatile uint8_t years[2], Month[2], hours[2], mintues[2], days[2], second[2];
	char * pch;
	pch = strstr (timerbuffer,"formatted");
	for(int j=0;j<33;j++){
	switch (j){
	case 14:
	years[0] = *(pch);break;
	case 15:
	years[1] = *(pch);break;
	case 17:
	Month[0] = *(pch);break;
	case 18:
	Month[1] = *(pch);break;
	case 20:
	days[0] = *(pch);break;
	case 21:
	days[1] = *(pch);break;
	case 23:
	hours[0] = *(pch);break;
	case 24:
	hours[1] = *(pch);break;
	case 26:
	mintues[0] = *(pch);break;
	case 27:
	mintues[1] = *(pch);break;
	case 29:
	second[0] = *(pch);break;
	case 30:
	second[1] = *(pch);break;
	default:
	    break;}
		*pch++;
	}
	years_int = atoi(years);
	sprintf(buf,"Value years is %d \n\r",years_int);
	HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	Month_int = atoi(Month);
	sprintf(buf,"Value  Mon:%d \n\r",Month_int);HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	days_int = atoi(days);
	sprintf(buf,"Value  days:%d \n\r",days_int);HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	hours_int = atoi(hours);
	sprintf(buf,"Value  ho:%d \n\r",hours_int);HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	mintues_int = atoi(mintues);
	sprintf(buf,"Value mintues is %d \n\r",mintues_int);
	HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	second_int = atoi(second);
	sprintf(buf,"Value second is %d \n\r",second_int);
	HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);

	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	 {
	   Error_Handler();
	 }

	sDate.Year=years_int;
	sDate.Month=Month_int;
	sDate.Date=days_int;

	if(HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BIN) != HAL_OK)
	 {
	   Error_Handler();
	 }
	sTime.Hours=hours_int;
	sTime.Minutes=mintues_int;
	sTime.Seconds=second_int;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
	  Error_Handler();
	}

    example_do_connect(client, "MYDEVICE3");

}
void fetc(void){

	int i;
	 int r;
	//httpd_init();
	char url[500];
	char domain_name[30];
	volatile uint16_t h=0;
	httpc_connection_t *conn_settings;
	httpc_state_t *connection;
	conn_settings->use_proxy = 0;   //no proxy usage
	conn_settings->headers_done_fn = RecvHttpHeaderCallback;
	conn_settings->result_fn = HttpClientResultCallback;
	strcpy(domain_name, "api.timezonedb.com");
	strcpy(url, "/v2.1/get-time-zone?key=H77HZBELV1YH&format=json&by=zone&zone=Africa/Cairo");
	sprintf(buf,"url:\n%s\n", url);
	HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	HAL_UART_Transmit(&huart3,"\n\r",2,1000);
	err_t error;
	//dns_gethostbyname();
	error = httpc_get_file_dns(domain_name, 80, url, conn_settings,RecvpayloadCallback, NULL, &connection);   //Send HTTP message
	sprintf(buf,"\n\rerror: %d\n", error);
	HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	HAL_Delay(5000);
	/*while(timerequestflag!=true){
		HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
		HAL_Delay(50);
		h++;
		if(h>1000){
			sprintf(buf,"timeout with dns \n");
			HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
			 timerequestflag=true;
		}
	}*/


}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	RTC_CalendarShow(aShowTime, aShowDate);
	if(timerequestflag==1){
	    example_do_connect(client, "MYDEVICE3");

		sprintf(pacote,"hello_mohamed from Stm32");


		example_publish(client, pacote);
	}

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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
