#include "sht31.h"
#include <stdio.h>
#include "debug.h"
#include "i2c.h"
#include "stm32f0xx.h"
#include "timer.h"

#define RD_TH_HIGH_HOLD_H 0x2c
#define RD_TH_HIGH_HOLD_L 0x06
#define RD_TH_MED_HOLD_H 0x2c
#define RD_TH_MED_HOLD_L 0x0D
#define RD_TH_LOW_HOLD_H 0x2c
#define RD_TH_LOW_HOLD_L 0x10
#define RD_TH_HIGH_NOHOLD_H 0x24
#define RD_TH_HIGH_NOHOLD_L 0x00
#define RD_TH_MED_NOHOLD_H 0x24
#define RD_TH_MED_NOHOLD_L 0x0B
#define RD_TH_LOW_NOHOLD_H 0x24
#define RD_TH_LOW_NOHOLD_L 0x16
#define HEATER_ENABLE_H 0x30
#define HEATER_ENABLE_L 0x6D
#define HEATER_DISABLE_H 0x30
#define HEATER_DISABLE_L 0x66
#define RD_STATUS_H 0xF3
#define RD_STATUS_L 0x2D
#define CLR_STATUS_H 0x30
#define CLR_STATUS_L 0x41
#define RESET_H 0x30
#define RESET_L 0xA2

int32_t Sht31Init(uint8_t addr) {
  int32_t rval = 0;
  int16_t status = 0;

  do {
    rval = Sht31Reset(addr);
    if (rval != 0) {
      dprint(ERR, "SHT could not reset %d\n", rval);
      break;
    }

    // Device requires 500us. 2ms is the smallest we can do with SleepMs
    SleepMs(2);

    rval = Sht31Status(addr, &status);
    if (rval != 0) {
      dprint(ERR, "SHT could not read status\n");
      break;
    }

    if ((status & 0x10) == 0) {
      dprint(ERR, "SHT invalid status\n");
      rval = SHT31_ERR;
      break;
    }
  } while (0);

  return rval;
}

int32_t Sht31Status(uint8_t addr, int16_t *status) {
  int32_t rval = 0;
  uint8_t rBuff[2];
  uint8_t wBuff[2] = {RD_STATUS_H, RD_STATUS_L};

  rval = i2c(SHT31_I2Cx, addr, 2, wBuff, 2, rBuff);

  if ((rval == I2C_OK) && (status != NULL)) {
    *status = (rBuff[0] << 8) | rBuff[1];
  }

  return rval;
}

int32_t Sht31Reset(uint8_t addr) {
  int32_t rval = 0;
  uint8_t wBuff[2] = {RESET_H, RESET_L};

  rval = i2c(SHT31_I2Cx, addr, 2, wBuff, 0, NULL);

  return rval;
}

int32_t Sht31Heater(uint8_t addr, bool enable) {
  int32_t rval = 0;
  uint8_t wBuff[2];

  if (enable) {
    wBuff[0] = HEATER_ENABLE_H;
    wBuff[1] = HEATER_ENABLE_L;
  } else {
    wBuff[0] = HEATER_DISABLE_H;
    wBuff[1] = HEATER_DISABLE_L;
  }

  rval = i2c(SHT31_I2Cx, addr, 2, wBuff, 0, NULL);

  return rval;
}

int32_t Sht31Read(uint8_t addr, int16_t *temperature, int16_t *humidity) {
  int32_t rval = 0;
  uint8_t rBuff[6];
  uint8_t wBuff[2] = {RD_TH_HIGH_NOHOLD_H, RD_TH_HIGH_NOHOLD_L};

  do {
    // Send read command
    rval = i2c(SHT31_I2Cx, addr, 2, wBuff, 0, NULL);
    if (rval != I2C_OK) {
      break;
    }

    SleepMs(15);  // Wait for measurement

    // Read back measurements
    rval = i2c(SHT31_I2Cx, addr, 0, NULL, 6, rBuff);
    if (rval != I2C_OK) {
      break;
    }

    if (temperature != NULL) {
      uint32_t temp;
      // Get temperature * 100
      temp = (rBuff[0] << 8) | rBuff[1];
      temp = -4500 + (temp * 17500) / 0xFFFF;
      *temperature = (uint16_t)temp;
    }

    if (humidity != NULL) {
      uint32_t rh;
      // Get humidity * 100
      rh = (rBuff[3] << 8) | rBuff[4];
      rh = (10000 * rh) / 0xFFFF;
      *humidity = (uint16_t)rh;
    }

  } while (0);

  return rval;
}
