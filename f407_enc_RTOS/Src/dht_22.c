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
extern TIM_HandleTypeDef htim4;

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
	delay_Timer(htim4,50);
	HAL_GPIO_WritePin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin, 1);   // pull the pin high
//	DWT_Delay_us (30);   // wait for 30us
	delay_Timer(htim4,3);
	set_gpio_input ();   // set as input
}

void check_response (void)
{
	//DWT_Delay_us (40);
	delay_Timer(htim4,4);

	if (!(HAL_GPIO_ReadPin (DHT_22_IN_OUT_GPIO_Port, DHT_22_IN_OUT_Pin)))
	{
	//	DWT_Delay_us (80);
		delay_Timer(htim4,8);

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
		delay_Timer(htim4,4);

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

