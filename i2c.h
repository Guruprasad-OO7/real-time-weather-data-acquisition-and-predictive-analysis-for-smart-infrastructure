#ifndef I2C_H
#define I2C_H
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(uint8_t data);
void i2c_write_addr(uint8_t addr);
void i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t data);
void i2c_read_bytes(uint8_t addr, uint8_t *data, uint8_t len);
void i2c_read_reg(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len);
#endif
