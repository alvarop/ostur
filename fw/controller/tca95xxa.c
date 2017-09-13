#include "tca95xxa.h"
#include <stdio.h>
#include "i2c.h"
#include "stm32f0xx.h"

int32_t Tca95xxaSetChannel(uint8_t addr, uint8_t channel) {
  int32_t rval;
  uint8_t wBuff[] = {(1 << channel) & 0xFF};

  do {
    if (channel > TCA95XXA_CHANNELS) {
      rval = TCA95XXA_ERR_CHANNEL;
      break;
    }

    rval = i2c(TCA95XXA_I2Cx, addr, 1, wBuff, 0, NULL);
  } while (0);

  return rval;
}
