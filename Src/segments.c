#include "segments.h"

// 4 B12, 3 B13, 2 B14, 1 B15
// A A1, F A7, B A0, G A6, C A5, dot A4, D A3, E A2

#define SEG_A (1 << 1)
#define SEG_B (1 << 0)
#define SEG_C (1 << 5)
#define SEG_D (1 << 3)
#define SEG_E (1 << 2)
#define SEG_F (1 << 7)
#define SEG_G (1 << 6)
#define SEG_DOT (1 << 4)

uint8_t alphabet [10] =
{
    (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F),
    (SEG_B | SEG_C),
    (SEG_A | SEG_B | SEG_D | SEG_E | SEG_G),
    (SEG_A | SEG_B | SEG_C | SEG_D | SEG_G),
    (SEG_B | SEG_C | SEG_F | SEG_G),
    (SEG_A | SEG_C | SEG_D | SEG_F | SEG_G),
    (SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),
    (SEG_A | SEG_B | SEG_C),
    (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),
    (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G),
};

uint8_t digits [4] = {1, 2, 3, 4};

void sseg_output (void)
{
    static uint8_t current_segment = 0;

    GPIOB->ODR |= 0xF000; 
    GPIOB->ODR &= ~(1 << (current_segment + 12));

    GPIOA->ODR &= ~0x00FF;
    GPIOA->ODR |= alphabet[digits[3 - current_segment]];

    current_segment++;
    current_segment %= 4;
}

