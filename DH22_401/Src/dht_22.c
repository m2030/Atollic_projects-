/*#include "main.h"
uint32_t timercounter=0;
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void DHT22_Start (void)
{
	Set_Pin_Output(DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin); // set the pin as output
	HAL_GPIO_WritePin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin, 0);   // pull the pin low
	delay(1500);   // wait for > 1ms

	HAL_GPIO_WritePin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin, 1);   // pull the pin high
	delay (30);   // wait for 30us

	Set_Pin_Input(DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin);   // set as input
}

uint8_t DHT22_Check_Response (void)
{
	Set_Pin_Input(DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin);   // set as input
	uint8_t Response = 0;
	delay (40);  // wait for 40us
	if (!(HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin))) // if the pin is low
	{
		delay (80);   // wait for 80us

		if ((HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin))) Response = 1;  // if the pin is high, response is ok
		else Response = -1;
	}
	while ((HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)));   // wait for the pin to go low
	  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);

	return Response;
}

uint8_t DHT22_Read (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)));   // wait for the pin to go high
		delay (40);   // wait for 40 us

		if (!(HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)));  // wait for the pin to go low
	}

	return i;
}

*/
/*************************************
#include "main.h"
#include "dht_22.h"
extern GPIO_InitTypeDef GPIO_InitStruct;
uint8_t check = 0;
uint32_t timercounter=0;
extern TIM_HandleTypeDef htim2;

void set_gpio_output (void)
{
  GPIO_InitStruct.Pin = DHT_22_IN_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT_22_IN_OUT_GPIO_Port, &GPIO_InitStruct);
}

void set_gpio_input (void)
{
  GPIO_InitStruct.Pin = DHT_22_IN_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT_22_IN_OUT_GPIO_Port, &GPIO_InitStruct);
}
void DHT22_start (void)
{
	set_gpio_output ();  // set the pin as output
	HAL_GPIO_WritePin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin, 0);   // pull the pin low
//	DWT_Delay_us (500);   // wait for 500us
	delay_Timer(htim2,50);
	HAL_GPIO_WritePin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin, 1);   // pull the pin high
//	DWT_Delay_us (30);   // wait for 30us
	delay_Timer(htim2,3);
	set_gpio_input ();   // set as input
}

void check_response (void)
{
	//DWT_Delay_us (40);
	delay_Timer(htim2,4);

	if (!(HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)))
	{
	//	DWT_Delay_us (80);
		delay_Timer(htim2,8);

		if ((HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin))) check = 1;
	}
	while ((HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)));   // wait for the pin to go low
}

uint8_t read_data (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)));   // wait for the pin to go high
		//DWT_Delay_us (40);   // wait for 40 us
		delay_Timer(htim2,4);

		if ((HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)) == 0)   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)));  // wait for the pin to go low
	}
	return i;
}
void delay_Timer(TIM_HandleTypeDef timer, uint32_t delay)
{
	 timercounter=0;

	  if(HAL_TIM_Base_Start_IT(&timer) != HAL_OK)
	  {
	    Error_Handler();
	  }
while(timercounter<delay)
	  {
		  HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);
	  }
if(__HAL_TIM_GET_FLAG(&timer, TIM_FLAG_UPDATE)!= RESET)
{
__HAL_TIM_CLEAR_IT(&timer, TIM_IT_UPDATE);
HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
}
HAL_TIM_Base_Stop(&timer);
HAL_TIM_Base_Stop_IT(&timer);

}**************************************/
/*
 * dht_22.c
 *
 *  Created on: Jul 14, 2019
 *      Author: fathy
 */
#include "main.h"
#include "dht_22.h"
extern GPIO_InitTypeDef GPIO_InitStruct;
uint8_t check = 0;
uint32_t timercounter=0;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;
uint8_t buf[200];
void set_gpio_output (void)
{
	/*Configure GPIO pin output: PA1 */
  GPIO_InitStruct.Pin = DHT_22_IN_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT_22_IN_OUT_GPIO_Port, &GPIO_InitStruct);
}

void set_gpio_input (void)
{
	/*Configure GPIO pin input: PA1 */
  GPIO_InitStruct.Pin = DHT_22_IN_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT_22_IN_OUT_GPIO_Port, &GPIO_InitStruct);
}
void DHT22_start (void)
{
	set_gpio_output ();  // set the pin as output
	HAL_GPIO_WritePin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin, 0);   // pull the pin low
//	DWT_Delay_us (500);   // wait for 500us
	delay(1200);
	HAL_GPIO_WritePin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin, 1);   // pull the pin high
//	DWT_Delay_us (30);   // wait for 30us
	delay(29);
	set_gpio_input ();   // set as input
}

void check_response (void)
{
	//DWT_Delay_us (40);
	delay(40);

	if (!(HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)))
	{
	//	DWT_Delay_us (80);
		delay(80);

		if ((HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin))) check = 1;
	}
	while ((HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)));   // wait for the pin to go low


}

uint8_t read_data (void)
{
	uint8_t i,j;

	for (j=0;j<8;j++)
	{

		while (!(HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)));   // wait for the pin to go high
		//DWT_Delay_us (40);   // wait for 40 us
		delay(40);
		if ((HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)) == 0)   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)));  // wait for the pin to go low
	}
	return i;
}
void delay_Timer(TIM_HandleTypeDef timer, uint32_t delay)
{
	 timercounter=0;

	  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
	  /* Start Channel1 */
	  if(HAL_TIM_Base_Start_IT(&timer) != HAL_OK)
	  {
	    /* Starting Error */
	    Error_Handler();
	  }
while(timercounter<delay)
	  {
		  HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);
	  }
if(__HAL_TIM_GET_FLAG(&timer, TIM_FLAG_UPDATE)!= RESET)
{
__HAL_TIM_CLEAR_IT(&timer, TIM_IT_UPDATE);
HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
}
HAL_TIM_Base_Stop(&timer);
HAL_TIM_Base_Stop_IT(&timer);

}
