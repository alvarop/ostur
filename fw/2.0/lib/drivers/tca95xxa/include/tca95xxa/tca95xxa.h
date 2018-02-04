#ifndef __TCA95XXA_H__
#define __TCA95XXA_H__

#include <stdint.h>

#define TCA95XXA_ADDR (0x70 << 1)

#define TCA95XXA_CHANNELS (4)

#define TCA95XXA_ERR_CHANNEL (-16)

int32_t tca95xxa_set_channel(uint8_t addr, uint8_t channel);

#endif
