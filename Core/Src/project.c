/*******************************************************************************
  * @file           : project.c
  * @brief          : The main program, it will get the time from the user and
  * 				  display it on the display.
  ******************************************************************************
  * @author	Daniel Gripenstedt
  ******************************************************************************
  */
#include "stdio.h"
#include "Project.h"
#include "main.h"
#include "rtc.h"
#include "string.h"
#include "usart.h"
#include "adc.h"
#include "time.h"
#include "tim.h"

/** @brief Project, this function will get input from the user via uart to
 * set the start time of the internal clock of the nucleo.
@author  Daniel Gripenstedt
@return void */
void Project(void)
{
	Init_display();
	Dim_display();
	Set_user_clock();	//get time form user via uart

	while(1)
	{
		Dim_display();	//control the red backlight brightness with PWM
		Display_clock();	//display the time
	}
}/*End of function Project*/
