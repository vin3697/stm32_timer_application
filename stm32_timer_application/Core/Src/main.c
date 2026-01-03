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

  system_init(&global_system);

  while (1)
  {


	  read_sensor_value(&global_system);

	  process_sensor_readings(&global_system);

	  system_mode_selection(&global_system);

	  global_system.current_cycle_mode	= global_system.system_mode;

	  change_in_mode(&global_system);

	  system_mode_operation(&global_system);


	  HAL_GPIO_WritePin(global_system.led.pin_base	, global_system.led.pin_number, global_system.led.pin_state);

	  HAL_UART_Transmit(&huart3,
			  	  	  	(uint8_t *)(&global_system.potentiometer.u8_percentage_value),
			  	  	  	sizeof(global_system.potentiometer.u8_percentage_value),
						100);

	  HAL_Delay(100);

	  global_system.last_cycle_mode	= global_system.current_cycle_mode;


  }

}

