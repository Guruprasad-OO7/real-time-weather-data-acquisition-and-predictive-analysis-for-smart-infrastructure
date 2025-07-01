#include "dht11.h"
#include "stm32f10x.h"
#include "delay.h"
#define DHT11_PORT GPIOA
#define DHT11_PIN  1
void dht11_init(void) 
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~(0xF << (DHT11_PIN * 4));
    GPIOA->CRL |= (0x3 << (DHT11_PIN * 4)); 
    GPIOA->ODR |= (1 << DHT11_PIN); 
}
static void dht11_pin_output(void) 
{
    GPIOA->CRL &= ~(0xF << (DHT11_PIN * 4));
    GPIOA->CRL |= (0x3 << (DHT11_PIN * 4)); 
}
static void dht11_pin_input(void) 
{
    GPIOA->CRL &= ~(0xF << (DHT11_PIN * 4));
    GPIOA->CRL |= (0x4 << (DHT11_PIN * 4)); 
}
static void dht11_start_signal(void)
{
    dht11_pin_output();
    GPIOA->ODR &= ~(1 << DHT11_PIN);  
    delay_ms(20);                    
    GPIOA->ODR |= (1 << DHT11_PIN);   
    delay_us(30);
    dht11_pin_input();
}
static uint8_t dht11_read_byte(void)
{
    uint8_t i, result = 0;
    for (i = 0; i < 8; i++) {
        while (!(GPIOA->IDR & (1 << DHT11_PIN))); 
        delay_us(40);  
        if (GPIOA->IDR & (1 << DHT11_PIN))
            result |= (1 << (7 - i)); 
        while (GPIOA->IDR & (1 << DHT11_PIN)); 
    }
    return result;
}

uint8_t dht11_read_data(uint8_t *temp, uint8_t *humidity) 
{
    uint8_t rh1, rh2, t1, t2, sum;
    dht11_start_signal();
    delay_us(40);
    if ((GPIOA->IDR & (1 << DHT11_PIN))) return 0;
    delay_us(80);
    if (!(GPIOA->IDR & (1 << DHT11_PIN))) return 0;
    delay_us(80);
    rh1 = dht11_read_byte();
    rh2 = dht11_read_byte();
    t1 = dht11_read_byte();
    t2 = dht11_read_byte();
    sum = dht11_read_byte();
    if ((rh1 + rh2 + t1 + t2) != sum) return 0;
    *humidity = rh1;
    *temp = t1;
    return 1;
}
