#ifndef __TCA9548A_H__
#define __TCA9548A_H__

#include <stdint.h>

#define TCA9548A_ADDR (0x70 << 1)

#define TCA9548A_CHANNELS (8)

#define TCA9548A_ERR_CHANNEL (-16)

#define TCA9548A_I2Cx I2C1

int32_t tca9584a_set_channel(uint8_t addr, uint8_t channel);

#endif