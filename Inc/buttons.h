#include <stdbool.h>
#include <stdint.h>
#include "stm32f103xb.h"

typedef enum
{
	LEFT_BUTTON = 0,
	MIDDLE_BUTTON = 1,
	RIGHT_BUTTON = 2
} pin_names_t;

typedef struct
{
	GPIO_TypeDef* port;
	uint32_t pin_num;
	bool push_active;
	bool push_unhandled;
	uint16_t push_time;
} button_t;

void buttons_process(void);
bool check_button (button_t* button);
extern button_t buttons [3];