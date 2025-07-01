#include "bmp280.h"
#include "i2c.h"
#include "delay.h"
#include <math.h>
#define BMP280_ADDR  0x76
#define BMP280_REG_TEMP_MSB  0xFA
#define BMP280_REG_CTRL_MEAS 0xF4
#define BMP280_REG_CONFIG    0xF5
static int32_t t_fine;
static int16_t dig_T1, dig_T2, dig_T3;
static int16_t dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
static uint16_t read16(uint8_t reg) {
    uint8_t buf[2];
    i2c_read_reg(BMP280_ADDR, reg, buf, 2);
    return (buf[1] << 8) | buf[0];
}
static void read_calibration_data(void) {
    dig_T1 = read16(0x88);
    dig_T2 = (int16_t)read16(0x8A);
    dig_T3 = (int16_t)read16(0x8C);
    dig_P1 = read16(0x8E);
    dig_P2 = (int16_t)read16(0x90);
    dig_P3 = (int16_t)read16(0x92);
    dig_P4 = (int16_t)read16(0x94);
    dig_P5 = (int16_t)read16(0x96);
    dig_P6 = (int16_t)read16(0x98);
    dig_P7 = (int16_t)read16(0x9A);
    dig_P8 = (int16_t)read16(0x9C);
    dig_P9 = (int16_t)read16(0x9E);
}
void bmp280_init(void)
{
    read_calibration_data();
    i2c_write_reg(BMP280_ADDR, BMP280_REG_CTRL_MEAS, 0x27); 
    i2c_write_reg(BMP280_ADDR, BMP280_REG_CONFIG, 0xA0);    
}
static int32_t bmp280_read_raw_temp(void) {
    uint8_t data[3];
    i2c_read_reg(BMP280_ADDR, BMP280_REG_TEMP_MSB, data, 3);
    return (int32_t)(((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | (data[2] >> 4));
}
static int32_t bmp280_read_raw_pressure(void) {
    uint8_t data[3];
    i2c_read_reg(BMP280_ADDR, 0xF7, data, 3);
    return (int32_t)(((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | (data[2] >> 4));
}
static float bmp280_compensate_temp(int32_t adc_T) {
    float var1 = (((float)adc_T) / 16384.0f - ((float)dig_T1) / 1024.0f) * ((float)dig_T2);
    float var2 = ((((float)adc_T) / 131072.0f - ((float)dig_T1) / 8192.0f) *
                 (((float)adc_T) / 131072.0f - ((float)dig_T1) / 8192.0f)) * ((float)dig_T3);
    t_fine = (int32_t)(var1 + var2);
    return (var1 + var2) / 5120.0f;
}
static float bmp280_compensate_pressure(int32_t adc_P)
{
    float var1 = ((float)t_fine / 2.0f) - 64000.0f;
    float var2 = var1 * var1 * ((float)dig_P6) / 32768.0f;
    var2 = var2 + var1 * ((float)dig_P5) * 2.0f;
    var2 = (var2 / 4.0f) + (((float)dig_P4) * 65536.0f);
    var1 = (((float)dig_P3) * var1 * var1 / 524288.0f + ((float)dig_P2) * var1) / 524288.0f;
    var1 = (1.0f + var1 / 32768.0f) * ((float)dig_P1);
    if (var1 == 0.0f) return 0; 
    float p = 1048576.0f - (float)adc_P;
    p = ((p - (var2 / 4096.0f)) * 6250.0f) / var1;
    var1 = ((float)dig_P9) * p * p / 2147483648.0f;
    var2 = p * ((float)dig_P8) / 32768.0f;
    return p + (var1 + var2 + ((float)dig_P7)) / 16.0f;
}
void bmp280_read_all(float *temp, float *pressure, float *altitude) 
{
    int32_t adc_T = bmp280_read_raw_temp();
    int32_t adc_P = bmp280_read_raw_pressure();
    *temp = bmp280_compensate_temp(adc_T);
    *pressure = bmp280_compensate_pressure(adc_P) / 100.0f; 
    *altitude = 44330.0f * (1.0f - powf(*pressure / 1013.25f, 0.1903f));
}
