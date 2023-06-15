#include "buttons.h"

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

void buttons_process(void)
{
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
}