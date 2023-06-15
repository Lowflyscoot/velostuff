#include "init.h"

void cpu_init (void);
void gpio_init (void);
void timers_init (void);

void init_mcu (void)
{
	cpu_init();
	gpio_init();
	timers_init();
}

void cpu_init (void)
{
    RCC->CR |= RCC_CR_HSEON | RCC_CR_PLLON;
    RCC->CFGR |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL4 | RCC_CFGR_SW_PLL;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_TIM1EN;
    //__ASM volatile ("cpsie i" : : : "memory");
    NVIC_EnableIRQ(TIM1_UP_IRQn);
}


void gpio_init (void)
{
	/*
	 * left red - A11, middle white - A9, reght red - A8
	 * left button - A12, middle button - A15, right button - A10
	 */
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
    GPIOC->ODR &= ~GPIO_ODR_ODR13;

    // Cleaning for pins 9, 10, 11, 12
    GPIOA->CRH &= ~((0x0F << 8) | (0x0F << 0) | (0x0F << 4) | (0x0F << 12)  | (0x0F << 16) | (0x0F << 28));
    // Pin 10 input with push pull, pin 9, 11, 12 push pull output
    GPIOA->CRH |= (0b0010 << 0) | (0b0010 << 4) | (0b0010 << 12);
    GPIOA->CRH |= (0b1000 << 8) | (0b1000 << 16) | (0b1000 << 28);

    // Input pin A10 pull-up set
    GPIOA->BSRR = 1 << 10;
    GPIOA->BRR |= ((1 << 8) | (1 << 9));
    GPIOA->BSRR |= (1 << 11);

    LEFT_LIGHT_RESET;
	MIDDLE_LIGHT_RESET;
	RIGHT_LIGHT_RESET;
}

void timers_init (void)
{
	TIM1->CR1 |= TIM_CR1_CEN;
	TIM1->ARR = 16000;
	//TIM1->PSC = 32000;

	TIM1->DIER |= 1;
}
