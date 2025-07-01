
#ifndef DHT11_H
#define DHT11_H
void dht11_init(void);
uint8_t dht11_read_data(uint8_t *temp, uint8_t *humidity);
#endif
