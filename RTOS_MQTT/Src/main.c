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
#include "cmsis_os.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <time.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include "lwip/apps/http_client.h"
#include "lwip/apps/mqtt.h"
#include "bh1750.h"
#include "st7735.h"
#include "fonts.h"
#include "stdio.h"
#include <stdbool.h>
/* MCP9808 high accuracy temp sensor from adafruit (no address pins pulled up) */
#define MCP9808_REG_TEMP (0x05) // Temperature Register
#define MCP9808_REG_CONF (0x01) // Configuration Register
#define MCP9808_ADDR     (0x30) // MCP9808 base address 0x18<<1
float BH1750_lux;
extern char buffer[1000];
volatile char TempCelsiusDisplay[] = "+abc.dd C";
RTC_TimeTypeDef currentTime;
RTC_DateTypeDef currentDate;
time_t timestamp;
struct tm currTime;
mqtt_client_t *client;
extern ip_addr_t mqttServerIP;
uint8_t MQTT_flag=0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart3;

osThreadId defaultTaskHandle;
osThreadId tempHandle;
osThreadId LightHandle;
uint8_t Value_json [200];
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM5_Init(void);
static void MX_RTC_Init(void);
void StartDefaultTask(void const * argument);
void Tasktemp(void const * argument);
void TaskLight(void const * argument);

/* USER CODE BEGIN PFP */
char buf[5000];
extern   ip_addr_t mqttServerIP;

void adjust_RTC(char *p);
void HttpClientResultCallback (void *arg, httpc_result_t httpc_result, u32_t
rx_content_len, u32_t srv_res, err_t err) ;
err_t RecvHttpHeaderCallback (httpc_state_t *connection, void *arg, struct
pbuf *hdr, u16_t hdr_len, u32_t content_len);
extern void serverFound(const char *name,ip_addr_t ipaddr, void *arg);
bool timerequestflag=false;
char timerbuffer[275];
uint8_t aShowTime[50] = {0};
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};
uint8_t aShowDate[50] = {0},callbackflag=1;
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
err_t RecvpayloadCallback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p,
		err_t err){
	//HAL_GPIO_TogglePin(LD1_GPIO_Port,LD1_Pin);
	sprintf(buf,"RecvpayloadCallback\n"	);
	 HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	 if (p == NULL) {sprintf(buf,"pbuf==Null\n");
	 HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	 }
	 else{
		 strncpy(timerbuffer, p->payload, 270);
		 sprintf(buf,"timerbuffer :%s\n\r",timerbuffer);
	HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	 }
	 timerequestflag=true;

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
	  uint8_t size;
	  float temprature;
	    char data_write[3];
	    char data_read[2];
	    int tempval;
  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface ytrnd the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 10240);
 defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of temp */
 osThreadDef(temp, Tasktemp, osPriorityNormal, 0, 2048);
tempHandle = osThreadCreate(osThread(temp), NULL);

  /* definition and creation of Light */
 //osThreadDef(Light, TaskLight, osPriorityNormal, 0, 1024);
 // LightHandle = osThreadCreate(osThread(Light), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
vTaskSuspend(tempHandle);

  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C2;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x20404768;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x20404768;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

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
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  htim2.Init.Prescaler = 215;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
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
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 215;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 21499;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 9999;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
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

  /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
  GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_VBUS_Pin */
  GPIO_InitStruct.Pin = USB_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_VBUS_GPIO_Port, &GPIO_InitStruct);
  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */

  volatile uint8_t years[2], Month[2], hours[2], mintues[2], days[2], second[2];
  MX_LWIP_Init();
//  httpd_init();
  client = mqtt_client_new();

  /* USER CODE BEGIN 5 */

  int i;
   int r;
  //httpd_init();

   uint16_t years_int, Month_int, hours_int, mintues_int, days_int, second_int;
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
  osDelay(5000);
  while(timerequestflag!=true){
  	HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
  	osDelay(50);
  	h++;
  	if(h>800){
  		sprintf(buf,"timeout with dns \n");
  		HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
  		 timerequestflag=true;
  	}
  }

  char * pch;
  h=0;
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
  sprintf(buf,"years Value is %d \n\r",years_int);
  HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
  Month_int = atoi(Month);
  sprintf(buf,"  Mon Value :%d \n\r",Month_int);HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
  days_int = atoi(days);
  sprintf(buf,"days Value :%d \n\r",days_int);HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
  hours_int = atoi(hours);
  sprintf(buf,"hour Value :%d \n\r",hours_int);HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
  mintues_int = atoi(mintues);
  sprintf(buf," mintues Value : %d \n\r",mintues_int);
  HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
  second_int = atoi(second);
  sprintf(buf," second Value: %d \n\r",second_int);
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
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */
  if (HAL_TIM_Base_Start_IT(&htim5) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
vTaskResume(tempHandle);
dns_gethostbyname("broker.hivemq.com", &mqttServerIP,serverFound);
example_do_connect(client, "MYDEVICE3");

  /* Infinite loop */
  for(;;)
  {
		   //osThreadTerminate(NULL);
		// RTC_CalendarShow(aShowTime, aShowDate);

	  if(callbackflag!=1){
if(MQTT_flag==1){
	//if(mqtt_client_is_connected(client)!=1){
  example_do_connect(client, "MYDEVICE3");//}
	example_publish(client,Value_json );

  MQTT_flag=0;
 HAL_UART_Transmit(&huart3,Value_json,strlen(Value_json),1000);
//sprintf(pacote,"hello_mohamed from Stm32");
    HAL_UART_Transmit(&huart3,"\n\r",2,1000);
	 HAL_GPIO_TogglePin(LD3_GPIO_Port,LD3_Pin);
	  sprintf(buffer,"mqttServerIP %d.%d.%d.%d\n\r",(mqttServerIP.addr & 0xff), ((mqttServerIP.addr >> 8) & 0xff), ((mqttServerIP.addr >> 16) & 0xff), (mqttServerIP.addr >> 24));
	  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);}
	// osDelay(1000);
  }}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Tasktemp */
/**
* @brief Function implementing the temp thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Tasktemp */
void Tasktemp(void const * argument)
{
  /* USER CODE BEGIN Tasktemp */
	vTaskSuspend(defaultTaskHandle);
	float temprature;
	char data_write[3];
	char data_read[2];
    int stat=0;
    int tempval;
HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
currTime.tm_year = currentDate.Year + 100;  // In fact: 2000 + 18 - 1900
currTime.tm_mday = currentDate.Date;
currTime.tm_mon  = currentDate.Month - 1;
currTime.tm_hour = currentTime.Hours;
currTime.tm_min  = currentTime.Minutes;
currTime.tm_sec  = currentTime.Seconds;
timestamp = mktime(&currTime);
if(HAL_I2C_Init(&hi2c1) != HAL_OK)
{
			    /* Initialization Error */
			//		sprintf(buf,"error Initiate  %d\r\n");
			//		HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
}
//  ST7735_Init();
BH1750_device_t* test_dev = BH1750_init_dev_struct(&hi2c1, "test device", true);
BH1750_init_dev(test_dev);
 //   ST7735_Init();
if(HAL_I2C_Init(&hi2c2) != HAL_OK)
	  {
	    /* Initialization Error */
		//	sprintf(buf,"error Initiate  %d\r\n");
		//	HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
}
//	sprintf(buf,"hello World\n\r");
	if(stat=(HAL_I2C_Master_Transmit(&hi2c2,MCP9808_ADDR,data_write,3,1000))!=HAL_OK)
	{
		sprintf(buf,"error sending commmand  %d\r\n",stat);
	HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	   while (1) {
  //   HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  HAL_Delay(200); }
}
  data_write[0] = MCP9808_REG_TEMP;
     HAL_I2C_Master_Transmit(&hi2c2,MCP9808_ADDR,data_write,1,1000);// no stop
    HAL_I2C_Master_Receive(&hi2c2,MCP9808_ADDR, data_read, 2,1000);
    if(data_read[0] & 0xE0) {
   	 sprintf(buf,"\r\n");
        HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
        data_read[0] = data_read[0] & 0x1F;  // clear flag bits
    }
    if((data_read[0] & 0x10) == 0x10) {
        data_read[0] = data_read[0] & 0x0F;
        TempCelsiusDisplay[0] = '-';
        tempval = 256 - (data_read[0] << 4) + (data_read[1] >> 4);
    } else {
        TempCelsiusDisplay[0] = '+';
        tempval = (data_read[0] << 4) + (data_read[1] >> 4);
    }

    // fractional part (0.25?C precision)
    if (data_read[1] & 0x08) {
        if(data_read[1] & 0x04) {
            TempCelsiusDisplay[5] = '7';
            TempCelsiusDisplay[6] = '5';
        } else {
            TempCelsiusDisplay[5] = '5';
            TempCelsiusDisplay[6] = '0';
        }
    } else {
        if(data_read[1] & 0x04) {
            TempCelsiusDisplay[5] = '2';
            TempCelsiusDisplay[6] = '5';
        }else{
            TempCelsiusDisplay[5] = '0';
            TempCelsiusDisplay[6] = '0';
        }
    }

    // Integer part
    TempCelsiusDisplay[1] = (tempval / 100) + 0x30;
    TempCelsiusDisplay[2] = ((tempval % 100) / 10) + 0x30;
    TempCelsiusDisplay[3] = ((tempval % 100) % 10) + 0x30;
    HAL_UART_Transmit(&huart3,"\r\n",2,1000);
    // Display result
    sprintf(buf,"temp = %s\r\n", TempCelsiusDisplay);
    HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
    test_dev->polll(test_dev);
    timestamp = mktime(&currTime);
    sprintf(buf,"timestamp = %l\r\n", timestamp);
    HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
    sprintf(Value_json,"json format : {\"LUX\": %d ,\"temperature\":%s ,\"timestamp\": %l}",test_dev->value,TempCelsiusDisplay, timestamp);
    HAL_UART_Transmit(&huart3,Value_json,strlen(Value_json),1000);
    timestamp = mktime(&currTime);
    sprintf(Value_json,"json format : {\"LUX\": %d ,\"temperature\":%s ,\"timestamp\": %l}",test_dev->value,TempCelsiusDisplay, timestamp);
    HAL_UART_Transmit(&huart3,Value_json,strlen(Value_json),1000);
    vTaskResume(defaultTaskHandle);
  /* Infinite loop */
  for(;;)
  {
	      data_write[0] = MCP9808_REG_TEMP;
	      HAL_I2C_Master_Transmit(&hi2c2,MCP9808_ADDR,data_write,1,1000);// no stop
	     HAL_I2C_Master_Receive(&hi2c2,MCP9808_ADDR, data_read, 2,1000);
	     if(data_read[0] & 0xE0) {
	    //	 sprintf(buf,"\r\n");
	      //   HAL_UART_Transmit(&huart3,buf,strlen(buf),1000);
	         data_read[0] = data_read[0] & 0x1F;  // clear flag bits
	     }
	     if((data_read[0] & 0x10) == 0x10) {
	         data_read[0] = data_read[0] & 0x0F;
	         TempCelsiusDisplay[0] = '-';
	         tempval = 256 - (data_read[0] << 4) + (data_read[1] >> 4);
	     } else {
	         TempCelsiusDisplay[0] = '+';
	         tempval = (data_read[0] << 4) + (data_read[1] >> 4);
	     }

	     // fractional part (0.25?C precision)
	     if (data_read[1] & 0x08) {
	         if(data_read[1] & 0x04) {
	             TempCelsiusDisplay[5] = '7';
	             TempCelsiusDisplay[6] = '5';
	         } else {
	             TempCelsiusDisplay[5] = '5';
	             TempCelsiusDisplay[6] = '0';
	         }
	     } else {
	         if(data_read[1] & 0x04) {
	             TempCelsiusDisplay[5] = '2';
	             TempCelsiusDisplay[6] = '5';
	         }else{
	             TempCelsiusDisplay[5] = '0';
	             TempCelsiusDisplay[6] = '0';
	         }
	     }

	     // Integer part
	     TempCelsiusDisplay[1] = (tempval / 100) + 0x30;
	     TempCelsiusDisplay[2] = ((tempval % 100) / 10) + 0x30;
	     TempCelsiusDisplay[3] = ((tempval % 100) % 10) + 0x30;
	  //   HAL_UART_Transmit(&huart3,"\r\n",2,1000);
	     // Display result
		    HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
		    HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);

		    currTime.tm_year = currentDate.Year + 100;  // In fact: 2000 + 18 - 1900
		    currTime.tm_mday = currentDate.Date;
		    currTime.tm_mon  = currentDate.Month - 1;

		    currTime.tm_hour = currentTime.Hours;
		    currTime.tm_min  = currentTime.Minutes;
		    currTime.tm_sec  = currentTime.Seconds;

		    timestamp = mktime(&currTime);
	     test_dev->polll(test_dev);
	     timestamp = mktime(&currTime);
	     sprintf(Value_json,"{\"LUX\": %d ,\"temperature\":%s ,\"timestamp\": %d}",test_dev->value,TempCelsiusDisplay, timestamp);
	   //  HAL_UART_Transmit(&huart3,Value_json,strlen(Value_json),1000);
       osDelay(996);
  }
  /* USER CODE END Tasktemp */
}

/* USER CODE BEGIN Header_TaskLight */
/**
* @brief Function implementing the Light thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TaskLight */
void TaskLight(void const * argument)
{
  /* USER CODE BEGIN TaskLight */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END TaskLight */
}

/* MPU Configuration */

void MPU_Config(void)
{

	MPU_Region_InitTypeDef MPU_InitStruct;
	/* Disable the MPU */
	HAL_MPU_Disable();
	/* Configure the MPU as Normal Non Cacheable for Ethernet Buffers in the SRAM2 */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x2007C000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	/* Configure the MPU as Device for Ethernet Descriptors in the SRAM2 */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x2007C000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER1;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

		  /* Enable the MPU */
		  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);


}
/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM3 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

	  if (htim->Instance == TIM5) {
		  MQTT_flag=1;
	  }
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM3) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
