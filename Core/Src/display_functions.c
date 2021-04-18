/*******************************************************************************
  * @file           : display_functions.c
  * @brief          : Functions for the display.
  ******************************************************************************
  * @author	Daniel Gripenstedt
  ******************************************************************************
  */

#include <stdio.h>
#include <main.h>
#include <spi.h>
#include"display_functions.h"
#include "adc.h"
#include "tim.h"


/** @brief SPI_send_display, position the bits correctly
 * and send 3 bytes over SPI to the display
 * @param uint8_t rs, uint8_t rw, uint8_t data
@author  Daniel Gripenstedt
@return void */
void SPI_send_display (uint8_t rs, uint8_t rw, uint8_t data)
{
	uint8_t dataBytes[3];	//3 bytes to transmit over SPI
	uint8_t startData = 0x00;
	uint8_t startByte = 0x1F;
	uint8_t firstByte = 0x0F;
	uint8_t secondByte = 0x0F;
	uint8_t data_byte = 0x00;

	if(rw == 1)
	{
		startByte = startByte | 0x20;	//set rw bit to 1
	}

	if(rs == 1)
	{
		startByte = startByte | 0x40;	//set rs bit to 1
	}

	data_byte = (data_byte | data);	//save the data in a varible

	firstByte = 0x0F & data_byte;	//mask the first byte

	data_byte = (data & 0xF0); // Mask last 4 bits
	secondByte = 0x0F & (data_byte >> 4);	//shift and store in second byte

	dataBytes[0] = startByte;
	dataBytes[1] = firstByte;
	dataBytes[2] = secondByte;

	HAL_GPIO_WritePin(Disp_CS_GPIO_Port, Disp_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, dataBytes, 3, 50);
	HAL_GPIO_WritePin(Disp_CS_GPIO_Port, Disp_CS_Pin, GPIO_PIN_SET);
}/* End of function SPI_send_display */



/** @brief Backlight_color, set backlight color
 * 	color = 0 --> turn off backlight
 * 	color = 1 --> Red	red does not work now when it is controlled by PWM
 * 	color = 2 --> Green
 * 	color = 3 --> White
 *	@param uint8_t color
@author  Daniel Gripenstedt
@return void */
void Backlight_color(uint8_t color)
{
	if(color == 0)
	{
		//HAL_GPIO_WritePin(Disp_Red_GPIO_Port, Disp_Red_Pin, GPIO_PIN_RESET);
		//HAL_Delay(10);

		HAL_GPIO_WritePin(Disp_Green_GPIO_Port, Disp_Green_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);

		HAL_GPIO_WritePin(Disp_White_GPIO_Port, Disp_White_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
	}

	/*if(color == 1)
	{
		//HAL_GPIO_WritePin(Disp_Red_GPIO_Port, Disp_Red_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
	}*/

	if(color == 2)
		{
			HAL_GPIO_WritePin(Disp_Green_GPIO_Port, Disp_Green_Pin, GPIO_PIN_SET);
			HAL_Delay(10);
		}

	if(color == 3)
		{
			HAL_GPIO_WritePin(Disp_White_GPIO_Port, Disp_White_Pin, GPIO_PIN_SET);
			HAL_Delay(10);
		}
} /* End of function Backlight_color */


/** @brief Init_display, Initiate the display,
 * the display can't print any characters without this.
 * However, the backlight will work without this function.
@author  Daniel Gripenstedt
@return void */
void Init_display(void)
{
	HAL_Delay(10);
	HAL_GPIO_WritePin(Disp_Reset_GPIO_Port, Disp_Reset_Pin, GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(Disp_Reset_GPIO_Port, Disp_Reset_Pin, GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(Disp_Reset_GPIO_Port, Disp_Reset_Pin, GPIO_PIN_SET);
	HAL_Delay(500);

	/*Commands that will initialize the display*/
	SPI_send_display (0x00, 0x00, 0x3A);
	SPI_send_display (0x00, 0x00, 0x09);
	SPI_send_display (0x00, 0x00, 0x06);
	SPI_send_display (0x00, 0x00, 0x1E);
	SPI_send_display (0x00, 0x00, 0x39);
	SPI_send_display (0x00, 0x00, 0x1B);
	SPI_send_display (0x00, 0x00, 0x6E);
	SPI_send_display (0x00, 0x00, 0x56);
	SPI_send_display (0x00, 0x00, 0x7A);
	SPI_send_display (0x00, 0x00, 0x38);
	SPI_send_display (0x00, 0x00, 0x0C);
	SPI_send_display (0x00, 0x00, 0x01);

	HAL_Delay(1000);
}/*End of function Init_display*/


/** @brief Write_display, display the character that
 * has the code "data" on the display.
 *	@param uint8_t data
@author  Daniel Gripenstedt
@return void */
void Write_display(uint8_t data)
{
	SPI_send_display(0x01, 0x00, data);
}/*End of function Write_display*/



/** @brief Dim_display, dim the red backlight using PWM.
 * The amount of voltage is regulated by PWM which will vary
 * depending on the value from ADC1_In16 which is determined by the
 * potentiometer.
@author  Daniel Gripenstedt
@return void */
void Dim_display(void)
{
	//uint16_t adc_value;
	uint16_t dim_value;
	HAL_ADC_Start(&hadc1);	//start adc
		while (HAL_ADC_PollForConversion(&hadc1, 2000) != HAL_OK);	//wait until adc value is red

		dim_value = HAL_ADC_GetValue(&hadc1);	//store adc value in adc_value
		if (dim_value < 40)	//to prevent negative values
		{
			dim_value = 0;
		}

		else
		{
			dim_value = ((dim_value / 40) - 1);	//store value between 0-99 in dim_value

			if (dim_value > 99)
					{
						dim_value = 99;
					}
		}

		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, dim_value);	//change the duty cycle
}/*End of function Dim_display*/


