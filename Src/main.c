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

void control_buttons(void);

uint8_t turn_left = 0;
uint8_t turn_right = 0;
uint16_t delay_cnt = 0;
uint8_t flash_cnt = 0;


typedef struct
{
	GPIO_TypeDef* port;
	uint32_t pin_num;
	bool push_active;
	bool push_unhandled;
	uint16_t push_time;
} button_t;

typedef enum
{
	LEFT_BUTTON = 0,
	MIDDLE_BUTTON = 1,
	RIGHT_BUTTON = 2
} pin_names_t;

button_t buttons [3] = 
{
	[LEFT_BUTTON] = {.port = GPIOA, .pin_num = 1 << 12, .push_active = false, .push_unhandled = false, .push_time = 0},
	[MIDDLE_BUTTON] = {.port = GPIOA, .pin_num = 1 << 15, .push_active = false, .push_unhandled = false, .push_time = 0},
	[RIGHT_BUTTON] = {.port = GPIOA, .pin_num = 1 << 10, .push_active = false, .push_unhandled = false, .push_time = 0}
};

bool check_button (button_t* button)
{
	return !(button->port->IDR & button->pin_num);
}

void TIM1_UP_IRQHandler (void)
{
	TIM1->SR = 0;

	for (int i = 0; i < 3; i++)
	{
		button_t* button = &buttons[i];

		if (check_button(button))
		{
			if (button->push_time < 0xFFFF) button->push_time++;
		}
		else
		{
			button->push_time = 0;
			button->push_active = false;
		}

		if (!button->push_active && button->push_time > 30)
		{
			button->push_active = true;
			button->push_unhandled = true;
		}
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
					MIDDLE_LIGHT_SWITCH;
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
