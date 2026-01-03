/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
 */

#include "gloabal_header_file.h"

uint8_t g_debug_variable	= 0;

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

	  system_execution(&global_system);

	  send_info_on_bus(&global_system);

	  global_system.last_cycle_mode	= global_system.current_cycle_mode;

	  HAL_Delay(100);


  }

}

