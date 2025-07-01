#include "bh1750.h"
#include "i2c.h"
#include "delay.h"
#define BH1750_ADDR  0x23
void bh1750_init(void) 
{
    i2c_start();
    i2c_write_addr(BH1750_ADDR);
    i2c_write(0x01);   
    i2c_stop();
    delay_ms(10);
    i2c_start();
    i2c_write_addr(BH1750_ADDR);
    i2c_write(0x10);   
    i2c_stop();
}
uint16_t bh1750_read_lux(void) 
{
    uint8_t data[2];
    i2c_read_bytes(BH1750_ADDR, data, 2);
    uint16_t raw = (data[0] << 8) | data[1];
    return (uint16_t)(raw / 1.2);  
}
