/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
 */

#include "gloabal_header_file.h"


int main(void)
{


  initialization();

  uint32_t u32_poti_analog_value 	= 0;
  uint8_t u8_poti_percentage_value 	= 0;

  while (1)
  {

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);

	  u32_poti_analog_value = HAL_ADC_GetValue(&hadc1);

	  u8_poti_percentage_value = percentage_calculation_u32(	0,
			  	  	  	  	  	  								4095,
																u32_poti_analog_value);

	  HAL_UART_Transmit(&huart3, (uint8_t *)(&u8_poti_percentage_value), sizeof(u8_poti_percentage_value), 100);

	  HAL_Delay(100);
	  // HAL_UART_Transmit(&huart3, (uint8_t *)(&u32_poti_analog_value), sizeof(u32_poti_analog_value), 100);

  }
}

