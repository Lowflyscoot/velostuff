/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>
#include "stm32f103xb.h"
#include "init.h"
#include "buttons.h"
#include "segments.h"

void control_buttons(void);

uint8_t turn_left = 0;
uint8_t turn_right = 0;
uint16_t delay_cnt = 0;
uint8_t flash_cnt = 0;
uint32_t circle_time = 0;


void TIM1_UP_IRQHandler (void)
{
	TIM1->SR = 0;

	buttons_process();

	sseg_output();

	circle_time++;
	if (circle_time > 10000)
	{
		circle_time = 0;
		digits [0] = 0;
		digits [1] = 0;
		digits [2] = 0;
		digits [3] = 0;
	}

	if (turn_left && flash_cnt < 20)
	{
		delay_cnt++;
		if (delay_cnt >= 200)
		{
			LEFT_LIGHT_SWITCH;
			delay_cnt = 0;
			flash_cnt++;
		}
		RIGHT_LIGHT_RESET;
	}
	else if (turn_right && flash_cnt < 20)
	{
		delay_cnt++;
		if (delay_cnt >= 200)
		{
			RIGHT_LIGHT_SWITCH;
			delay_cnt = 0;
			flash_cnt++;
		}
		LEFT_LIGHT_RESET;
	}
	else
	{
		turn_left = turn_right = 0;
		delay_cnt = 0;
		flash_cnt = 0;
		LEFT_LIGHT_RESET;
		RIGHT_LIGHT_RESET;
	}
}

int main(void)
{
    init_mcu();
    while (1)
    {
    	control_buttons();
    }
}

void control_buttons(void)
{
	float f_kmph = 0.0;
	uint32_t dw_kmph = 0;

	for (int i = 0; i < 3; i++)
	{
		button_t* button = &buttons[i];

		if (button->push_unhandled == true)
		{
			button->push_unhandled = false;
			switch (i)
			{
				case LEFT_BUTTON:
					turn_left ^= 1;
					turn_right = 0;
					flash_cnt = 0;
					break;
				case MIDDLE_BUTTON:
					f_kmph = 0.002 / (((float)circle_time) / 3600000);
					f_kmph *= 10;
					dw_kmph = (uint32_t)f_kmph;
					for (int j = 0; j < 4; j++)
					{
						digits [j] = dw_kmph % 10;
						dw_kmph /= 10;
					}
					circle_time = 0;
					break;
				case RIGHT_BUTTON:
					turn_right ^= 1;
					turn_left = 0;
					flash_cnt = 0;
					break;
				default:
					break;
			}
		}
	}
}
