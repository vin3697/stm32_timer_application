/*
 * algorithm_header.h
 *
 *  Created on: 3 Jan 2026
 *      Author: VIN
 */

#ifndef ALGORITHM_ALGORITHM_HEADER_H_
#define ALGORITHM_ALGORITHM_HEADER_H_

	#include "gloabal_header_file.h"


	typedef enum
	{
		work_mode 		= 0 ,
		inactive_mode		,
		error_mode			,
	}system_mode_t;

	// as sensor
	typedef struct
	{
		  uint32_t 	u32_analog_value 	;
		  uint8_t 	u8_percentage_value	;

	}potentiometer_t;

	// as actuator
	//typedef struct
	//{};

	typedef struct
	{
		GPIO_TypeDef* pin_base;
		uint16_t pin_number;
		GPIO_PinState pin_state;
	}led_operations_t;

	typedef enum
	{

		clock_wise_rotation		= 0	,
		anticlock_wise_rotatin		,
		motor_inactive

	}motor_rotation_direction_t;

	typedef struct
	{
		GPIO_TypeDef* 	pin_base;

		uint16_t 		pin_in1;
		GPIO_PinState	pin_in1_state;
		uint16_t 		pin_in2;
		GPIO_PinState	pin_in2_state;
		uint16_t 		pin_in3;
		GPIO_PinState	pin_in3_state;
		uint16_t 		pin_in4;
		GPIO_PinState	pin_in4_state;

		uint8_t 			u8_stepper_motor_time;

	}stepper_motor_t;

	// global structure for system
	typedef struct
	{
		system_mode_t		current_cycle_mode;
		system_mode_t		last_cycle_mode;

		system_mode_t		system_mode;
		potentiometer_t		potentiometer;

		led_operations_t	led;
		motor_rotation_direction_t	motor_rotation_direction;

		stepper_motor_t		stepper_motor;

		uint8_t 			u8_system_cycle_time;

	}global_system_t;



	// function declaration
	void system_init(global_system_t *const arg_global_system);
	void read_sensor_value(global_system_t *const arg_global_system);
	void process_sensor_readings(global_system_t *const arg_global_system);
	void system_mode_selection(global_system_t *const arg_global_system);
	void system_mode_operation(global_system_t *const arg_global_system);
	void change_in_mode(global_system_t *const arg_global_system);
	void work_mode_operation(global_system_t *const arg_global_system);
	void system_execution(global_system_t *const arg_global_system);
	void clockwise_movement(global_system_t *const arg_global_system);
	void anitclockwise_movement(global_system_t *const arg_global_system);
	void energize_pins( global_system_t *const arg_global_system);
	void stepper_motor_sequence(uint8_t u8_arg_step_id, global_system_t *const arg_global_system);
	void send_info_on_bus( global_system_t *const arg_global_system);
	void reset_cycle_count(global_system_t *const arg_global_system);
	void increment_cycle_count(global_system_t *const arg_global_system);
	void uC_cycle_time_op(global_system_t *const arg_global_system);
	void uC_timing_operations(global_system_t *const arg_global_system);




	// library functions
	uint32_t percentage_calculation_u32(uint32_t min_val,
										uint32_t max_val,
										uint32_t actual_val);


#endif /* ALGORITHM_ALGORITHM_HEADER_H_ */
