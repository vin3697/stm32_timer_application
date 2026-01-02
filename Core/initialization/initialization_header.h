/*
 * initialization_header.h
 *
 *  Created on: 2 Jan 2026
 *      Author: vinay
 */

#ifndef INITIALIZATION_INITIALIZATION_HEADER_H_
#define INITIALIZATION_INITIALIZATION_HEADER_H_


	#include "global_header_file.h"

	void Error_Handler(void);
	void MX_GPIO_Init(void);
	void MX_USART3_UART_Init(void);
	void MX_TIM2_Init(void);

	void SystemClock_Config(void);
	void MPU_Config(void);
	void MX_ADC1_Init(void);

	void initialization();

#endif /* INITIALIZATION_INITIALIZATION_HEADER_H_ */
