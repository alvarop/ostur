#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>

typedef struct {
  uint8_t addr;
  uint8_t bus;
} __attribute__((packed)) th_sensor_t;

#define CONFIG_MAX_SENSORS (8)

// WARNING: when changing struct, Cortex-m0 doesn't do unaligned access!!
typedef struct {
  uint32_t magic;                          // Magic number for configuration
  uint32_t period_ms;                      // Sampling period in ms
  int16_t temp_set;                        // Temperature setting
  int16_t humid_set;                       // Humidity setting
  th_sensor_t sensor[CONFIG_MAX_SENSORS];  // T/H Sensors
  uint8_t primary_sensor;                  // Primary sensor for TH control
  uint8_t outside_sensor;                  // Outside sensor for reference
  uint16_t flags;                          // Various flags
  uint16_t crc16;                          // CRC of configuration
} __attribute__((packed)) config_t;

#define CONFIG_FLAG_AUTOSTART (1 << 0)

int32_t ConfigInit();
config_t *ConfigGet();
void ConfigWrite();
void ConfigPrint();
void ConfigCmd(uint8_t argc, char *argv[]);

#endif
