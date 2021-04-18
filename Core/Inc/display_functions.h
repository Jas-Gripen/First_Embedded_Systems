/*******************************************************************************
  * @file           : display_functions.h
  * @brief          : Header file for display_functions.c
  ******************************************************************************
  * @author	Daniel Gripenstedt
  ******************************************************************************
  */
#include <stdio.h>
void SPI_send_display (uint8_t rw, uint8_t rs, uint8_t data);
void Backlight_color(uint8_t color);
void Init_display(void);
void Dim_display(void);
