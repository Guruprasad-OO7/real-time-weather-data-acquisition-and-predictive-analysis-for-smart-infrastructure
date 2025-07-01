#include "spi.h"
#include "stm32f10x.h"
void spi_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_SPI1EN;
    GPIOA->CRL &= ~((0xF << 20) | (0xF << 24) | (0xF << 28));
    GPIOA->CRL |=  (0xB << 20) | (0x4 << 24) | (0xB << 28); 
    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI |
                SPI_CR1_BR_0 | SPI_CR1_BR_1;  
    SPI1->CR1 |= SPI_CR1_SPE; 
}
uint8_t spi_transfer(uint8_t data) {
    while (!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;
    while (!(SPI1->SR & SPI_SR_RXNE));
    return SPI1->DR;
}
