#ifndef BMP280_H
#define BMP280_H
void bmp280_init(void);
void bmp280_read_all(float *temp, float *pressure, float *altitude);
#endif
