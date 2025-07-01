#include "i2c.h"
#include "stm32f10x.h"
void i2c_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    GPIOB->CRL &= ~((0xF << 24) | (0xF << 28));
    GPIOB->CRL |=  (0xF << 24) | (0xF << 28); 
    I2C1->CR2 = 36;          
    I2C1->CCR = 180;        
    I2C1->TRISE = 37;
    I2C1->CR1 = I2C_CR1_PE;
}
void i2c_start(void) {
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));
}
void i2c_stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
}
void i2c_write_addr(uint8_t addr) {
    I2C1->DR = addr << 1;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;
}
void i2c_write(uint8_t data) {
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF));
}

void i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t data) {
    i2c_start();
    i2c_write_addr(addr);
    i2c_write(reg);
    i2c_write(data);
    i2c_stop();
}
void i2c_read_bytes(uint8_t addr, uint8_t *data, uint8_t len) {
    i2c_start();
    I2C1->DR = (addr << 1) | 1; // Read
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    for (uint8_t i = 0; i < len; i++) {
        if (i == len - 1) I2C1->CR1 &= ~I2C_CR1_ACK;
        while (!(I2C1->SR1 & I2C_SR1_RXNE));
        data[i] = I2C1->DR;
    }
    I2C1->CR1 |= I2C_CR1_ACK;
    i2c_stop();
}

void i2c_read_reg(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len) {
    i2c_start();
    i2c_write_addr(addr);
    i2c_write(reg);
    i2c_stop();
    i2c_read_bytes(addr, buf, len);
}
