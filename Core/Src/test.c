/*******************************************************************************
  * @file           : test.c
  * @brief          : Test program for the main program. This code will test
  * 				  the different program functions. Selecting one function
  * 				  is done by commenting out the others.
  ******************************************************************************
  * @author	Daniel Gripenstedt
  ******************************************************************************
  */


#include "stdio.h"
#include "main.h"
#include "rtc.h"
#include "string.h"
#include "usart.h"
#include "adc.h"
#include "time.h"
#include "tim.h"


/** @brief Test_program, test functions to be used in the project
 * and some extra functions to better understand how different commands
 * and hardware works.
@author  Daniel Gripenstedt
@return void */
void Test_program(void)
{
	//Test_Display_Backlight();
	//Test_Display_Characters();
	//Test_uart();
	//Test_numbers();
	//Test_adc();
	//Test_clock();
	//Test_PWM();
	//Test_Dim_display();
	Test_combo();
}/*End of function Test_program*/



/** @brief Test_Display_Backlight, test the display
 * backlight colors, RED, GREEN and WHITE
@author  Daniel Gripenstedt
@return void */
void Test_Display_Backlight (void)
{
	/*this foor loop was used when PWM was not activated
	 * for(int i = 1; i <= 3; i++)
	{
		Backlight_color(i);	// set backlight color
		HAL_Delay(1000);
		Backlight_color(0);	// Turn off backlight
		HAL_Delay(1000);
	}*/

	Backlight_color(0);	// turn off backlight
	HAL_Delay(1000);
	Backlight_color(2);	// Set color green
	HAL_Delay(1000);

	Backlight_color(0);	// Turn off backlight
	HAL_Delay(1000);
	Backlight_color(3);	// Set color white
	HAL_Delay(1000);

	Backlight_color(0);	// Turn off backlight
	HAL_Delay(1000);

}/* End of Test_Display_Backlight */



/** @brief Test_Display_Characters, display different characters
 * 	on the display. The initialization sequence will write out characters.
@author  Daniel Gripenstedt
@return void */
void Test_Display_Characters (void)
{
	Backlight_color(2);
	HAL_Delay(10);
	Init_display();
} /* End function Test_Display_Characters */



/** @brief Test_uart, test sending and receiving data
 * with uart and seriell cable to pc
@author  Daniel Gripenstedt
@return void */
void Test_uart(void)
{
	uint8_t Tecken;
	uint8_t value;
	uint16_t size = 0x1;
	 uint8_t uart[] = "Start Test_uart\n\r";
	uint8_t true[] = "Tecken = 10 is true\n\r";

	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_UART5_Init();

	HAL_UART_Transmit(&huart5, (uint8_t*)uart, sizeof(uart), 5000);
	while (1)   {
		while(HAL_UART_Receive(&huart5, &Tecken, size, 5000) != HAL_OK);
		//HAL_UART_Transmit(&huart5, (uint8_t *)&Tecken, 1, 5000);

		if (HAL_UART_Receive(&huart5, &Tecken, size, 5000) != HAL_OK)
		{
			Test_Error_Handler();
		}

		/*if (HAL_UART_Transmit(&huart5, (uint8_t *)&Tecken, 1, 5000) != HAL_OK)
		{
			Test_Error_Handler();
		}*/

		sscanf ((uint8_t *)&Tecken,"%hhu", &value);
		if (Tecken == 10)
		{
			HAL_UART_Transmit(&huart5, (uint8_t *)&true, sizeof(true), 5000);
			HAL_Delay(7000);
		}
	}
}/*End of function Test_uart*/



/** @brief Test_Error_handler, error handler
 * to be used with uart in test program
@author  Daniel Gripenstedt
@return void */
void Test_Error_Handler(void)
{
	char *error = "Error handle\n\r";
	HAL_UART_Transmit(&huart5, (uint8_t*)error, strlen(error), 5000);
	HAL_Delay(5000);
	Test_uart();
}/*End of function Test_Error_Handler*/



/** @brief Test_numbers, test function Print_number(i)
 * which should print the number "i" on the display
@author  Daniel Gripenstedt
@return void */
void Test_numbers (void)
{
	for(int i = 0; i <= 9; i++)
	{
		Print_number(i);
		HAL_Delay(1000);
	}
}/*End of function Test_numbers*/



/** @brief Test_adc, test function to learn how adc works
 * get adc value and print a statement on pc via uart
 * depending on the adc value. The adc value can be changed with the potentiometer.
@author  Daniel Gripenstedt
@return void */
void Test_adc (void)
{
	uint16_t adc_value;
	char *print_adc;

	while(1)
	{
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 2000) == HAL_OK)	//wait for adc value
		{
			adc_value = HAL_ADC_GetValue(&hadc1);
			HAL_UART_Transmit(&huart5, (uint8_t *)&adc_value, 1, 5000);

			if (HAL_UART_Transmit(&huart5, (uint8_t *)&adc_value, 1, 5000) != HAL_OK)
				{
					Test_Error_Handler();
				}
			HAL_Delay(1000);
		}

		if (adc_value <= 400)
		{
			 print_adc = "adc_value <= 400\n\r";
			 HAL_UART_Transmit(&huart5, (uint8_t*)print_adc, strlen(print_adc), 5000);
			 HAL_Delay(5000);
		}

		if ((adc_value >= 400) && (adc_value <= 800))
		{
			print_adc = "400 <= adc_value <= 800\n\r";
			HAL_UART_Transmit(&huart5, (uint8_t*)print_adc, strlen(print_adc), 5000);
			HAL_Delay(5000);
		}
		if ((adc_value > 800) && (adc_value <= 1200))
		{
			print_adc = "800 <= adc_value <= 1200\n\r";
			HAL_UART_Transmit(&huart5, (uint8_t*)print_adc, strlen(print_adc), 5000);
			HAL_Delay(5000);
		}
		if ((adc_value > 1200) && (adc_value <= 1600))
		{
			print_adc = "1200 <= adc_value <= 1600\n\r";
			HAL_UART_Transmit(&huart5, (uint8_t*)print_adc, strlen(print_adc), 5000);
			HAL_Delay(5000);
		}
		if ((adc_value > 1600) && (adc_value <= 2000))
		{
			print_adc = "1600 <= adc_value <= 2000\n\r";
			HAL_UART_Transmit(&huart5, (uint8_t*)print_adc, strlen(print_adc), 5000);
			HAL_Delay(5000);
		}
		if ((adc_value > 2000) && (adc_value <= 2000))
		{
			print_adc = " 2000 < adc_value\n\r";
			HAL_UART_Transmit(&huart5, (uint8_t*)print_adc, strlen(print_adc), 5000);
			HAL_Delay(5000);
		}
	}
}/*End of function Test_adc*/



/** @brief Test_clock, test the clock
 * Set_user_clock() sets the time of the clock
 * Display_clock() displays the time on the display.
@author  Daniel Gripenstedt
@return void */
void Test_clock (void)
{
	Init_display();
	Backlight_color(2);
	HAL_Delay(50);
	Set_user_clock();

	while(1)
	{
		Display_clock();
	}
}/*End of function Test_clock*/


/** @brief Test_PWM, test the PWM by dimming
 * the red backlight
@author  Daniel Gripenstedt
@return void */
void Test_PWM (void)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	for(int i = 0; i <= 99; i++)
	{
		/*change duty cycle, i = 0 --> 0%,  i = 99 --> 100% */
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, i); //change duty cycle
		HAL_Delay(100);
	}
}/*End of function Test_PWM*/


/*@brief Test_Dim_display, test the function Dim_display().
 * Dim_display() will dim the red backlight with PWM depending
 * on the ADC value which can be manually changed with the potentiometer.
@author  Daniel Gripenstedt
@return void */
void Test_Dim_display(void)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	while(1)
		{
			Dim_display();
		}
}/*End of function Test_dim_display*/

/** @brief Test_comb, will test showing the time and let the user dim the display
 * at the same time
@author  Daniel Gripenstedt
@return void */
void Test_combo (void)
{
	Init_display();
	Dim_display();
	Set_user_clock();

	while(1)
	{
		Dim_display();
		Display_clock();
	}
}/*End of function Test_combo*/
