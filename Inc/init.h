#include <stdint.h>
#include "stm32f103xb.h"

#define LEFT_PUSH !(GPIOA->IDR & (1 << 12))
#define MIDDLE_PUSH !(GPIOA->IDR & (1 << 15))
#define RIGHT_PUSH !(GPIOA->IDR & (1 << 10))

#define LEFT_LIGHT_SET (GPIOA->BSRR = 1 << 11)
#define LEFT_LIGHT_RESET (GPIOA->BRR = 1 << 11)
#define MIDDLE_LIGHT_SET (GPIOA->BSRR = 1 << 9)
#define MIDDLE_LIGHT_RESET (GPIOA->BRR = 1 << 9)
#define RIGHT_LIGHT_SET (GPIOA->BSRR = 1 << 8)
#define RIGHT_LIGHT_RESET (GPIOA->BRR = 1 << 8)

#define LEFT_LIGHT_SWITCH (GPIOA->ODR ^= 1 << 11)
#define MIDDLE_LIGHT_SWITCH (GPIOA->ODR ^= 1 << 9)
#define RIGHT_LIGHT_SWITCH (GPIOA->ODR ^= 1 << 8)

void init_mcu (void);
