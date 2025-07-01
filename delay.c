#include "delay.h"
#include "stm32f10x.h"
static volatile uint32_t tick_us = 0;
void delay_init(void) {
    SysTick->LOAD = 72 - 1; 
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}
void delay_us(uint32_t us)
{
    while (us--) {
        tick_us = 0;
        SysTick->VAL = 0;
        while (SysTick->VAL < 72); 
    }
}
void delay_ms(uint32_t ms) {
    while (ms--) delay_us(1000);
}
