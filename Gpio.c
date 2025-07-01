#include "gpio.h"
void gpio_init(void) 
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    gpio_pin_mode(GPIOA, 8, INPUT_FLOATING);
    gpio_pin_mode(GPIOA, 11, INPUT_FLOATING);
}
void gpio_pin_mode(GPIO_TypeDef *port, uint8_t pin, uint8_t mode) 
{
    uint32_t shift = (pin % 8) * 4;
    volatile uint32_t *cr = (pin < 8) ? &port->CRL : &port->CRR;

    if (pin < 8) {
        *cr &= ~(0xF << shift);
        *cr |= (mode << shift);
    } else {
        cr = &port->CRH;
        *cr &= ~(0xF << shift);
        *cr |= (mode << shift);
    }
    
}
uint8_t read_gpio_pin(GPIO_TypeDef *port, uint8_t pin) 
{
    return (port->IDR & (1 << pin)) ? 1 : 0;
}
