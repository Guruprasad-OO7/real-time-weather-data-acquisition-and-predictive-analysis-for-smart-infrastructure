#ifndef GPIO_H
#define GPIO_H
#include "stm32f10x.h"
#define INPUT_FLOATING     0x4
#define OUTPUT_2MHZ_PP     0x2
void gpio_init(void);
void gpio_pin_mode(GPIO_TypeDef *port, uint8_t pin, uint8_t mode);
uint8_t read_gpio_pin(GPIO_TypeDef *port, uint8_t pin);
#endif
