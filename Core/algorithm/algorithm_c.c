/*
 * algorithm_c.c
 *
 *  Created on: 3 Jan 2026
 *      Author: VIN
 */




#include "gloabal_header_file.h"

// system instance
global_system_t	global_system;


// -----------------------------------------------------------------------

void system_init(global_system_t *const arg_global_system)
{

	arg_global_system->current_cycle_mode 	= 	inactive_mode;
	arg_global_system->last_cycle_mode		=	inactive_mode;

	arg_global_system->system_mode			= 	inactive_mode;

	arg_global_system->led.pin_base			=	GPIOB;
	arg_global_system->led.pin_number		= 	LD2_Pin;
	arg_global_system->led.pin_state		=	GPIO_PIN_SET;

	arg_global_system->motor_rotation_direction				= motor_inactive;

	arg_global_system->potentiometer.u32_analog_value		= 0;
	arg_global_system->potentiometer.u8_percentage_value	= 0;

	arg_global_system->stepper_motor.pin_base		= GPIOF;
	arg_global_system->stepper_motor.pin_in1		= GPIO_PIN_0;
	arg_global_system->stepper_motor.pin_in2		= GPIO_PIN_1;
	arg_global_system->stepper_motor.pin_in3		= GPIO_PIN_2;
	arg_global_system->stepper_motor.pin_in4		= GPIO_PIN_3;

	arg_global_system->stepper_motor.pin_in1_state	= GPIO_PIN_RESET;
	arg_global_system->stepper_motor.pin_in2_state	= GPIO_PIN_RESET;
	arg_global_system->stepper_motor.pin_in3_state	= GPIO_PIN_RESET;
	arg_global_system->stepper_motor.pin_in4_state	= GPIO_PIN_RESET;
	arg_global_system->stepper_motor.u8_stepper_motor_time	= 0;

	arg_global_system->u8_system_cycle_time			= 0;

	return;
}


// -----------------------------------------------------------------------

void read_sensor_value(global_system_t *const arg_global_system)
{
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);

	  arg_global_system->potentiometer.u32_analog_value = HAL_ADC_GetValue(&hadc1);

	return;
}



void process_sensor_readings(global_system_t *const arg_global_system)
{
	const uint16_t u16_poit_min_value = 0;
	const uint16_t u16_poit_max_value = 4095;

	arg_global_system->potentiometer.u8_percentage_value = percentage_calculation_u32(	u16_poit_min_value,
																						u16_poit_max_value,
																						arg_global_system->potentiometer.u32_analog_value);
	return;
}


void system_mode_selection(global_system_t *const arg_global_system)
{
	uint8_t u8_system_mode_value	= 0;
	u8_system_mode_value			= arg_global_system->potentiometer.u8_percentage_value;

	const uint8_t error_mode_threshold		= 80;
	const uint8_t inactive_mode_threshold	= 20;

	if(	u8_system_mode_value >= error_mode_threshold )
	{
		arg_global_system->system_mode	= error_mode;
	}
	else if( u8_system_mode_value <= inactive_mode_threshold  )
	{
		arg_global_system->system_mode	= inactive_mode;
	}
	else
	{
		arg_global_system->system_mode	= work_mode;
	}

	return;
}

// -----------------------------------------------------------------------

void send_info_on_bus( global_system_t *const arg_global_system)
{
	const uint8_t tx_uart	= 3;
	HAL_UART_Transmit(&huart3,
				  	  (uint8_t *)(&tx_uart),
					  sizeof(tx_uart),
					  100);
	  /*HAL_UART_Transmit(&huart3,
			  	  	  	(uint8_t *)(&arg_global_system->potentiometer.u8_percentage_value),
			  	  	  	sizeof(arg_global_system->potentiometer.u8_percentage_value),
						100);*/
	return;
}

void energize_pins( global_system_t *const arg_global_system)
{

	HAL_GPIO_WritePin(	arg_global_system->stepper_motor.pin_base	,
						arg_global_system->stepper_motor.pin_in1	,
						arg_global_system->stepper_motor.pin_in1_state);

	HAL_GPIO_WritePin(	arg_global_system->stepper_motor.pin_base	,
						arg_global_system->stepper_motor.pin_in2	,
						arg_global_system->stepper_motor.pin_in2_state);

	HAL_GPIO_WritePin(	arg_global_system->stepper_motor.pin_base	,
						arg_global_system->stepper_motor.pin_in3	,
						arg_global_system->stepper_motor.pin_in3_state);

	HAL_GPIO_WritePin(	arg_global_system->stepper_motor.pin_base	,
						arg_global_system->stepper_motor.pin_in4	,
						arg_global_system->stepper_motor.pin_in4_state);


}


void stepper_motor_sequence(uint8_t u8_arg_step_id, global_system_t *const arg_global_system)
{


	arg_global_system->stepper_motor.pin_in1_state	= GPIO_PIN_RESET;
	arg_global_system->stepper_motor.pin_in2_state	= GPIO_PIN_RESET;
	arg_global_system->stepper_motor.pin_in3_state	= GPIO_PIN_RESET;
	arg_global_system->stepper_motor.pin_in4_state	= GPIO_PIN_RESET;

	 switch (u8_arg_step_id)
	    {
	        case 0U: /* 1000 */
	        	arg_global_system->stepper_motor.pin_in1_state	= GPIO_PIN_SET;
	            break;

	        case 1U: /* 1100 */
	        	arg_global_system->stepper_motor.pin_in1_state	= GPIO_PIN_SET;
	        	arg_global_system->stepper_motor.pin_in2_state	= GPIO_PIN_SET;
	            break;

	        case 2U: /* 0100 */
	        	arg_global_system->stepper_motor.pin_in2_state	= GPIO_PIN_SET;
	            break;

	        case 3U: /* 0110 */
	        	arg_global_system->stepper_motor.pin_in2_state	= GPIO_PIN_SET;
	        	arg_global_system->stepper_motor.pin_in3_state	= GPIO_PIN_SET;
	        	break;

	        case 4U: /* 0010 */
	        	arg_global_system->stepper_motor.pin_in3_state	= GPIO_PIN_SET;
	            break;

	        case 5U: /* 0011 */
	        	arg_global_system->stepper_motor.pin_in3_state	= GPIO_PIN_SET;
	        	arg_global_system->stepper_motor.pin_in4_state	= GPIO_PIN_SET;
	        	break;

	        case 6U: /* 0001 */
	        	arg_global_system->stepper_motor.pin_in4_state	= GPIO_PIN_SET;
	            break;

	        case 7U: /* 1001 */
	        	arg_global_system->stepper_motor.pin_in1_state	= GPIO_PIN_SET;
	        	arg_global_system->stepper_motor.pin_in4_state	= GPIO_PIN_SET;
	            break;

	        default:
	            /* Invalid index -> all outputs off */
	            break;
	    }

	 energize_pins(arg_global_system);

	 return;
}



void anitclockwise_movement(global_system_t *const arg_global_system)
{

	static uint8_t u8_s_step = 0;
	if(u8_s_step >=8)
	{
		u8_s_step = 0;
	}
	stepper_motor_sequence(u8_s_step, arg_global_system);
	u8_s_step++;

    /*uint8_t u8_step = 0;

    for (u8_step = 0U; u8_step < 8U; u8_step++)
    {

    	stepper_motor_sequence(u8_step, arg_global_system);
    	HAL_Delay(3);
    }*/

    return;
}

void clockwise_movement(global_system_t *const arg_global_system)
{

	static int8_t i8_s_step = 7;
	if(i8_s_step <= 0)
	{
		i8_s_step = 7;
	}
	stepper_motor_sequence(i8_s_step, arg_global_system);
	i8_s_step--;

	/*uint8_t u8_step_id = 0;

    for (u8_step_id = 0U; u8_step_id < 8U; u8_step_id++)
    {
        uint8_t u8_step = 0;
        u8_step			= 7 - u8_step_id;

        stepper_motor_sequence(u8_step, arg_global_system);
    	HAL_Delay(3);

    }*/

    return;
}

void system_execution(global_system_t *const arg_global_system)
{


	switch(arg_global_system->motor_rotation_direction)
	{
		case clock_wise_rotation:
		{

			clockwise_movement(arg_global_system);
			break;
		}

		case anticlock_wise_rotatin:
		{
			anitclockwise_movement(arg_global_system);
			break;
		}

		default: // motor_inactive
			// as of now do nothing
			arg_global_system->stepper_motor.pin_in1_state	= GPIO_PIN_RESET;
			arg_global_system->stepper_motor.pin_in2_state	= GPIO_PIN_RESET;
			arg_global_system->stepper_motor.pin_in3_state	= GPIO_PIN_RESET;
			arg_global_system->stepper_motor.pin_in4_state	= GPIO_PIN_RESET;
			energize_pins(arg_global_system);

			break;
	}

	// set LED according to the Mode
	HAL_GPIO_WritePin(arg_global_system->led.pin_base	, arg_global_system->led.pin_number, arg_global_system->led.pin_state);

	return;
}



void change_in_mode(global_system_t *const arg_global_system)
{
	if( arg_global_system->current_cycle_mode != arg_global_system->last_cycle_mode)
	{
		// reset the LED Pin
		arg_global_system->led.pin_state	=	GPIO_PIN_RESET;

		HAL_GPIO_WritePin(arg_global_system->led.pin_base	, arg_global_system->led.pin_number, arg_global_system->led.pin_state);

	}
	else
	{
		// do nothing as of now
	}

	return;
}



void work_mode_operation(global_system_t *const arg_global_system)
{

	uint8_t u8_system_mode_value	= 0;
	u8_system_mode_value			= arg_global_system->potentiometer.u8_percentage_value;

	arg_global_system->led.pin_base		=	GPIOB;
	arg_global_system->led.pin_number	=	LD1_Pin;
	arg_global_system->led.pin_state	=	GPIO_PIN_SET;

	const uint8_t clockwise_min_threshold	= 30;
	const uint8_t clockwise_max_threshold	= 40;

	const uint8_t anticlockwise_min_threshold	= 60;
	const uint8_t anticlockwise_max_threshold	= 70;

	if( ( clockwise_min_threshold <= u8_system_mode_value ) && ( u8_system_mode_value <= clockwise_max_threshold))
	{
		arg_global_system->motor_rotation_direction	=	clock_wise_rotation;

		g_debug_variable	= 33;

	}
	else if( ( anticlockwise_min_threshold <= u8_system_mode_value ) && ( u8_system_mode_value <= anticlockwise_max_threshold))
	{
		arg_global_system->motor_rotation_direction	=	anticlock_wise_rotatin;
		g_debug_variable	= 66;

	}
	else{
		arg_global_system->motor_rotation_direction	= motor_inactive;
		g_debug_variable	= 99;
	}

}

void system_mode_operation(global_system_t *const arg_global_system)
{


	switch(arg_global_system->system_mode)
	{
		case work_mode:
		{
			// set LED pin

			work_mode_operation(arg_global_system);

			break;
		}

		case inactive_mode:
		{

			arg_global_system->led.pin_base		=	GPIOB;
			arg_global_system->led.pin_number	=	LD2_Pin;
			arg_global_system->led.pin_state	=	GPIO_PIN_SET;

			break;
		}

		case error_mode:
		{

			arg_global_system->led.pin_base		=	GPIOB;
			arg_global_system->led.pin_number	=	LD3_Pin;
			arg_global_system->led.pin_state	=	GPIO_PIN_SET;

			break;
		}

		default:
			break;
	}

	return;
}

// -----------------------------------------------------------------------

void increment_cycle_count(global_system_t *const arg_global_system)
{
	arg_global_system->u8_system_cycle_time	= 	arg_global_system->u8_system_cycle_time + 1;

	return;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{


  if (htim->Instance == TIM3)
  {
		increment_cycle_count(&global_system);
  }
  else{
	  // do nothing
  }

  // Timer for Stepper motor is possible as well

}

// -----------------------------------------------------------------------
// library functions
uint32_t percentage_calculation_u32(uint32_t min_val,
                                    uint32_t max_val,
                                    uint32_t actual_val)
{
    uint32_t percent_u32 = 0U;

    if (max_val > min_val)
    {
        uint32_t clamped_u32 = actual_val;

        if (clamped_u32 < min_val)
        {
            clamped_u32 = min_val;
        }
        else if (clamped_u32 > max_val)
        {
            clamped_u32 = max_val;
        }
        else
        {
            /* within range */
        }

        /* Range is guaranteed > 0 here */
        {
            uint32_t range_u32 = (max_val - min_val);
            uint32_t num_u32   = (clamped_u32 - min_val);

            /* Compute: round( num*100 / range )
             * Avoid overflow of num*100 using divide-first strategy.
             */
            uint32_t q_u32 = num_u32 / range_u32;      /* 0 or 1 after clamp */
            uint32_t r_u32 = num_u32 % range_u32;

            /* q_u32 is 0..1, so q_u32*100 is safe */
            percent_u32 = (q_u32 * 100U);

            /* Add fractional part with rounding */
            percent_u32 += (uint32_t)((r_u32 * 100U + (range_u32 / 2U)) / range_u32);
        }
    }
    else
    {
        /* invalid range: return 0 */
        percent_u32 = 0U;
    }

    /* Clamp to 100 just in case (should already be 0..100) */
    if (percent_u32 > 100U)
    {
        percent_u32 = 100U;
    }

    return percent_u32;
}
