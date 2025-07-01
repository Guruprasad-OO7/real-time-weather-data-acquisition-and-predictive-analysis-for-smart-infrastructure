#include "lora.h"
#include "spi.h"
#include "gpio.h"
#include "delay.h"
#include <string.h>
#define LORA_CS_PORT GPIOA
#define LORA_CS_PIN  4
static void lora_cs_high(void) 
{
    LORA_CS_PORT->ODR |= (1 << LORA_CS_PIN);
}
static void lora_cs_low(void) 
{
    LORA_CS_PORT->ODR &= ~(1 << LORA_CS_PIN);
}

static void lora_write_reg(uint8_t addr, uint8_t value)
{
    lora_cs_low();
    spi_transfer(addr | 0x80); 
    spi_transfer(value);
    lora_cs_high();
}

static uint8_t lora_read_reg(uint8_t addr) {
    lora_cs_low();
    spi_transfer(addr & 0x7F); 
    uint8_t value = spi_transfer(0x00);
    lora_cs_high();
    return value;
}

void lora_init(void) 
{
    gpio_pin_mode(LORA_CS_PORT, LORA_CS_PIN, OUTPUT_2MHZ_PP);
    lora_cs_high();
    delay_ms(100);
    lora_write_reg(0x01, 0x80); 
    delay_ms(10);
    lora_write_reg(0x01, 0x81); 
    lora_write_reg(0x06, 0x6C); 
    lora_write_reg(0x07, 0x80);
    lora_write_reg(0x08, 0x00);
    lora_write_reg(0x0E, 0x00); 
    lora_write_reg(0x0F, 0x00); 
}

void lora_send_string(const char *str)
{
    uint8_t len = strlen(str);
    lora_write_reg(0x0D, 0x00);
    lora_cs_low();
    spi_transfer(0x00 | 0x80); 
    for (uint8_t i = 0; i < len; i++)
      {
        spi_transfer(str[i]);
    }
    lora_cs_high();
    lora_write_reg(0x22, len);
    lora_write_reg(0x01, 0x83);
    while ((lora_read_reg(0x12) & 0x08) == 0);
    lora_write_reg(0x12, 0xFF);
}
