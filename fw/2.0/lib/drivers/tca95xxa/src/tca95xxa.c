#include <stdint.h>
#include <assert.h>
#include "console/console.h"
#include "i2c/i2c.h"
#include "tca95xxa/tca95xxa.h"

int32_t tca95xxa_set_channel(uint8_t addr, uint8_t channel) {

  int32_t rval;
  uint8_t wBuff[] = {(1 << channel) & 0xFF};

  do {
    if (channel > TCA95XXA_CHANNELS) {
      rval = TCA95XXA_ERR_CHANNEL;
      break;
    }

    rval = i2c(0, addr, 1, wBuff, 0, NULL, 10);
  } while (0);

  return rval;
}
