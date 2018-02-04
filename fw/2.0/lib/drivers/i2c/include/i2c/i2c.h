#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>

typedef enum {
    I2C_OK = 0,
    I2C_ANACK = -1,
    I2C_DNACK = -2,
    I2C_TIMEOUT = -3,
    I2C_ERR = -9,
} i2cReturn_t;

typedef enum {
    I2C_FREQ_100K = 100,
    I2C_FREQ_250K = 250,
    I2C_FREQ_400K = 400,
} i2c_freq_t;

int32_t i2c_init(  uint8_t i2c_num,
                uint8_t sda_pin,
                uint8_t scl_pin,
                i2c_freq_t freq);

int32_t i2c(uint8_t i2c_num,
            uint8_t addr,
            uint16_t wlen,
            uint8_t *wbuff,
            uint16_t rlen,
            uint8_t *rbuff,
            uint32_t timeout);

int32_t i2c_probe(uint8_t i2c_num, uint8_t addr, uint32_t timeout);

#endif
