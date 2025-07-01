#include "usart.h"
#include "stm32f10x.h"

void usart_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |= (0xB << 4); 
    USART1->BRR = 0x1D4C; 
    USART1->CR1 = USART_CR1_TE | USART_CR1_UE;
}
void uart_send_char(char c) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}
void uart_send_string(const char *s) 
{
    while (*s) {
        uart_send_char(*s++);
    }
}
