#ifndef __SHT31_H__
#define __SHT31_H__

#include <stdbool.h>
#include <stdint.h>

#define SHT31_ADDR (0x44 << 1)
#define SHT31_ALT_ADDR (0x45 << 1)

#define SHT31_I2Cx I2C1

#define SHT31_ERR (-14)

int32_t Sht31Init(uint8_t addr);
int32_t Sht31Status(uint8_t addr, int16_t *status);
int32_t Sht31Reset(uint8_t addr);
int32_t Sht31Heater(uint8_t addr, bool enable);
int32_t Sht31Read(uint8_t addr, int16_t *temp, int16_t *humidity);

#endif
