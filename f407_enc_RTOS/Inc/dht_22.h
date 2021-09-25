/*
 * dht_22.h
 *
 *  Created on: Jul 14, 2019
 *      Author: fathy
 */

#ifndef DHT_22_H_
#define DHT_22_H_
#include "main.h"
extern GPIO_InitTypeDef GPIO_InitStruct;

void set_gpio_output (void);
void set_gpio_input (void);
void DHT22_start (void);
void check_response (void);
uint8_t read_data (void);
void delay_Timer(TIM_HandleTypeDef timer, uint32_t delay);
#endif /* DHT_22_H_ */
