#include <stdio.h>
#include "tca9584a.h"
#include "i2c.h"
#include "stm32f0xx.h"

int32_t tca9584a_set_channel(uint8_t addr, uint8_t channel) {
	int32_t rval;
	uint8_t wBuff[] = {(1 << channel) & 0xFF};

	do {
		if (channel > 7) {
			rval = TCA9548A_ERR_CHANNEL;
			break;
		}

		rval = i2c(TCA9548A_I2Cx, addr, 1, wBuff, 0, NULL);
	} while(0);

	return rval;
}
