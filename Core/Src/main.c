


/**
  ******************************************************************************
  	  * @file           : main.c
	  * Creator 		: VIN
  ******************************************************************************

  */


#include "global_header_file.h"


/**
  * @brief  The application entry point.
  * @retval int
  */



int main(void)
{

	initialization();

	uint32_t u32_poti_analog_value = 0;


	while (1)
	{

		  HAL_ADC_PollForConversion(&hadc1, 10);
		  u32_poti_analog_value = HAL_ADC_GetValue(&hadc1);

		  HAL_UART_Transmit(&huart3, (uint8_t *)(&u32_poti_analog_value), 1, 100);

	}

}


