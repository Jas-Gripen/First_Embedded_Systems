/*******************************************************************************
  * @file           : time.c
  * @brief          : Functions to set the time (via serial cable and uart from
  * 					pc to nucleo) for the RTC clock and show it on the display.
  ******************************************************************************
  * @author	Daniel Gripenstedt
  ******************************************************************************
  */


#include <display_functions.h>
#include "time.h"
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "rtc.h"
#include "string.h"
#include "usart.h"

static RTC_TimeTypeDef sTime;
static RTC_DateTypeDef sDate;
static uint8_t prev_hour = 0;
static uint8_t prev_minute = 0;
static uint8_t prev_sec = 0;


/** @brief Display_number, display "number" on the display.
 * @param number
@author  Daniel Gripenstedt
@return void */
void Display_number(uint8_t number)
{
	if (number == 0)
	{
		Write_display(0x30);
	}

	if (number == 1)
	{
		Write_display(0x31);
	}

	if (number == 2)
	{
		Write_display(0x32);
	}

	if (number == 3)
	{
		Write_display(0x33);
	}

	if (number == 4)
	{
		Write_display(0x34);
	}

	if (number == 5)
	{
		Write_display(0x35);
	}

	if (number == 6)
	{
		Write_display(0x36);
	}

	if (number == 7)
	{
		Write_display(0x37);
	}

	if (number == 8)
	{
		Write_display(0x38);
	}

	if (number == 9)
	{
		Write_display(0x39);
	}
}/*End of function Display_number*/



/** @brief Large_number, if the number has 2 digits
 * then split it into to numbers, digit 1 and digit 2.
 * If the number has one digit, first digit becomes 0
 * second digit becoms the number. Send digits to Display_numbers(uint8_t number);
 * @param Large_number
@author  Daniel Gripenstedt
@return void */
void Large_number (uint8_t Large_number)
{
	double temp_num = Large_number / 10;	//first number

	uint8_t first_num = floor(temp_num);	//make temp_num into an uint8_t

	temp_num = Large_number;
	int modulo_num = Large_number % 10;	//modulos can not be made on uint8_t
	uint8_t second_num = modulo_num;


		Display_number(first_num);
		Display_number(second_num);
}/*End of function Large_number*/



/** @brief Set_user_clock, let the user set the time
 * via uart and serial cable from pc to nucleo.
@author  Daniel Gripenstedt
@return void */
void Set_user_clock(void)
{
	/*Display "Set hour"*/
	Write_display(0x53);
	Write_display(0x65);
	Write_display(0x74);
	Write_display(0x20);
	Write_display(0x68);
	Write_display(0x6F);
	Write_display(0x75);
	Write_display(0x72);

	RTC_TimeTypeDef sTime;	//struct to store time
	RTC_DateTypeDef sDate;	//struct to store dates

	/*Time varibles*/
	uint8_t prev_sec = 0;
	uint8_t uHour = 0;
	uint8_t uMinute = 0;
	uint8_t uSecond = 0;
	uint16_t size = 1;

	if (HAL_UART_Receive(&huart5, &uHour, size, 60000) != HAL_OK) // get hour from user
				{
					Error_Handler();
				}

	SPI_send_display(0,0,0xC0);	//rad 3
	Large_number(uHour);
	Write_display(0x3A);	// display :

	/*Display "Set minute"*/
	SPI_send_display(0x00, 0x00, 0x80);	//rad 1
	Write_display(0x53);
	Write_display(0x65);
	Write_display(0x74);
	Write_display(0x20);
	Write_display(0x6D);
	Write_display(0x69);
	Write_display(0x6E);
	Write_display(0x75);
	Write_display(0x74);

	SPI_send_display(0,0,0xC0);	//rad 3
	Large_number(uHour);
	Write_display(0x3A);

	if (HAL_UART_Receive(&huart5, &uMinute, size, 60000) != HAL_OK)	//get minute from user
	{
		Error_Handler();
	}

	//SPI_send_display(0,0,0xC0);
	Large_number(uMinute);
	Write_display(0x3A);

	/*Display "Set second"*/
	SPI_send_display(0x00, 0x00, 0x80);	//rad 1
	Write_display(0x53);
	Write_display(0x65);
	Write_display(0x74);
	Write_display(0x20);
	Write_display(0x73);
	Write_display(0x65);
	Write_display(0x63);

	Write_display(0x6F);
	Write_display(0x6E);
	Write_display(0x64);
	Write_display(0x73);

	SPI_send_display(0,0,0xC0);	//rad 3
	Large_number(uHour);
	Write_display(0x3A);
	Large_number(uMinute);
	Write_display(0x3A);

	if (HAL_UART_Receive(&huart5, &uSecond, size, 60000) != HAL_OK)	//get second from user
	{
		Error_Handler();
	}

	Large_number(uSecond);
	SPI_send_display(0x00, 0x00, 0x01);	//clear display
	/*Write "Time" on the display*/
	SPI_send_display(0x00, 0x00, 0x00);	//rad 1
	Write_display(0x54);
	Write_display(0x69);
	Write_display(0x6D);
	Write_display(0x65);

		/* Store the users time in sTime */
		sTime.Hours = uHour;
		sTime.Minutes = uMinute;
		sTime.Seconds = uSecond;

	    HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
	    HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
}/*End of function Set_user_clock*/



/** @brief Display_clock, display the time on the display
* @param number
@author  Daniel Gripenstedt
@return void */
void Display_clock(void)
{
	HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BIN);

	if(sTime.Hours == 0x18)	//fix bug where sTime.Hours get the value 24, reset back to 00:00:00
	{
		sTime.Hours = 0x00;
		sTime.Minutes = 0x00;
		sTime.Seconds = 0x00;
		HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
	}

	if((sTime.Hours == 0x01) && (prev_hour == 0x0C))	//fix bug where sTime.Hours goes from 12 to 01
	{
		sTime.Hours = 0x0D;
		sTime.Minutes = 0x00;
		sTime.Seconds = 0x00;
		HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
	}

	if(prev_sec != sTime.Seconds)	//if we have a new second, display the new time
	{
		SPI_send_display(0,0,0xC0);	//rad 3

		Large_number(sTime.Hours);
		Write_display(0x3A);	// display :
		Large_number(sTime.Minutes);
		Write_display(0x3A);
		Large_number(sTime.Seconds);
	}
	prev_hour = sTime.Hours;
	prev_sec = sTime.Seconds;
}/*End of function Display_clock*/

