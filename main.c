#include "dht11.h"
#include "bh1750.h"
#include "bmp280.h"
#include "lora.h"
#include "usart.h"
#include "delay.h"
#include "gpio.h"    
#include <stdio.h>   
int main(void) 
{
    uint8_t temp_dht = 0, hum_dht = 0;
    float temp_bmp = 0, pressure = 0, altitude = 0;
    uint16_t light_lux = 0;
    uint8_t rain_status = 0, snow_status = 0;
    char message[128];
    delay_init();         
    usart_init();      
    gpio_init();          
    dht11_init();         
    i2c_init();           
    bh1750_init();         
    bmp280_init();        
    spi_init();           
    lora_init();          

    uart_send_string("Weather Node Online...\r\n");

    while (1) {
        
        if (!dht11_read_data(&temp_dht, &hum_dht)) {
            uart_send_string("Error: DHT11 read failed\r\n");
            continue;  
        }
        light_lux = bh1750_read_lux();
        bmp280_read_all(&temp_bmp, &pressure, &altitude);
        rain_status = read_gpio_pin(GPIOA, 8);   
        snow_status = read_gpio_pin(GPIOA, 11);  
        snprintf(message, sizeof(message),
            "T:%dC H:%d%% L:%uLux TP:%.1fC P:%.1f hPa A:%.1f m Rain:%d Snow:%d",
            temp_dht, hum_dht, light_lux,
            temp_bmp, pressure, altitude,
            rain_status, snow_status);
        uart_send_string("Sending via LoRa: ");
        uart_send_string(message);
        uart_send_string("\r\n");
        lora_send_string(message);
        delay_ms(3000);
    }
}
DHT
#ifndef DHT11_H
#define DHT11_H
void dht11_init(void);
uint8_t dht11_read_data(uint8_t *temp, uint8_t *humidity);
#endif
