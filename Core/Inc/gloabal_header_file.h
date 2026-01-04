/*
 * gloabal_header_file.h
 *
 *  Created on: 3 Jan 2026
 *      Author: vinay
 */

#ifndef INC_GLOABAL_HEADER_FILE_H_
#define INC_GLOABAL_HEADER_FILE_H_

	#include "main.h"
	#include "init_header_file.h"
	#include "algorithm_header.h"

	extern ADC_HandleTypeDef 	hadc1;
	extern TIM_HandleTypeDef 	htim3;
	extern UART_HandleTypeDef 	huart3;

	// global visibility
	extern global_system_t		global_system;
	extern uint8_t 				g_debug_variable;

	#define CYCLE_TIME_FACTOR	1u

#endif /* INC_GLOABAL_HEADER_FILE_H_ */
