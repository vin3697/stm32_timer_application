/*
 * init_header_file.h
 *
 *  Created on: 3 Jan 2026
 *      Author: VIN
 */

#ifndef INITIALIZATION_INIT_HEADER_FILE_H_
#define INITIALIZATION_INIT_HEADER_FILE_H_

	#include "gloabal_header_file.h"


	void SystemClock_Config(void);
	void MPU_Config(void);
	void MX_GPIO_Init(void);
	void MX_ADC1_Init(void);
	void MX_TIM3_Init(void);
	void MX_USART3_UART_Init(void);

	void initialization(void);

#endif /* INITIALIZATION_INIT_HEADER_FILE_H_ */
