#include <stdint.h>
#include <hal/hal_i2c.h>
#include <mcu/nrf52_hal.h>
#include <i2c/i2c.h>

int32_t i2c_init(uint8_t i2c_num, uint8_t sda_pin, uint8_t scl_pin,
                 i2c_freq_t freq) {

    struct nrf52_hal_i2c_cfg cfg = {
        .scl_pin = scl_pin,
        .sda_pin = sda_pin,
        .i2c_frequency = freq
    };

    return hal_i2c_init(i2c_num, (void *)&cfg);
}

int32_t i2c(uint8_t i2c_num, uint8_t addr, uint16_t wlen, uint8_t *wbuff,
            uint16_t rlen, uint8_t *rbuff, uint32_t timeout) {

    int32_t rval = -1;

    do {
        if(wlen) {
            struct hal_i2c_master_data pdata = {
                .address = addr,
                .len = wlen,
                .buffer = wbuff
            };
            rval = hal_i2c_master_write(i2c_num, &pdata,
                             timeout, (rlen == 0));

            if (rval) {
                break;
            }
        }

        if(rlen) {
            struct hal_i2c_master_data pdata = {
                .address = addr,
                .len = rlen,
                .buffer = rbuff
            };
            rval = hal_i2c_master_read(i2c_num, &pdata,
                             timeout, 1);

            if (rval) {
                break;
            }
        }
    } while (0);

    // TODO - change errors to i2c specific ones

    return rval;
}

int32_t i2c_probe(uint8_t i2c_num, uint8_t addr, uint32_t timeout) {
    return hal_i2c_master_probe(i2c_num, addr, timeout);
}
