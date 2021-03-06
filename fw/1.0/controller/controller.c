#include "controller.h"
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "config.h"
#include "debug.h"
#include "i2c.h"
#include "rtc.h"
#include "sht31.h"
#include "stm32f0xx_conf.h"
#include "tca95xxa.h"
#include "timer.h"

typedef struct {
  int16_t temperature;
  int16_t humidity;
} __attribute__((packed)) th_value_t;

// NOTE: MUST be a power of 2
#define CONTROLLER_BUFF_SAMPLES (64)

// Time the fridge must be OFF before turning on again
#define FRIDGE_OFF_TIME_M (15)

static bool running = false;
static config_t *config;
static ms_timer_t controller_timer;
static ms_timer_t fridge_off_timer;
static th_value_t primary_buff[CONTROLLER_BUFF_SAMPLES];
static th_value_t outside_buff[CONTROLLER_BUFF_SAMPLES];
static uint16_t buff_index;

int32_t ControllerInit() {
  int32_t rval;
  config = ConfigGet();
  dprint(DEBUG, "%s\n", __func__);
  for (uint8_t sensor_id = 0; sensor_id < CONFIG_MAX_SENSORS; sensor_id++) {
    th_sensor_t *sensor = &config->sensor[sensor_id];
    if (sensor->addr != 0) {
      rval = Tca95xxaSetChannel(TCA95XXA_ADDR, sensor->bus);
      if (rval != 0) {
        dprint(ERR, "could not set i2c bus (%ld)\n", rval);
        GPIO_ResetBits(TCA_nRST_PORT, (1 << TCA_nRST_PIN));
        SleepMs(2);
        GPIO_SetBits(TCA_nRST_PORT, (1 << TCA_nRST_PIN));
        i2cSetup(100000);
        rval = Tca95xxaSetChannel(TCA95XXA_ADDR, sensor->bus);
        dprint(ERR, "Retry %d\n", rval);
        if (rval != 0) {
          break;
        }
        break;
      }

      Sht31Init(sensor->addr);
      if (rval != 0) {
        dprint(ERR, "SHT could not initialize (%ld)\n", rval);
        break;
      }
    }
  }

  for (uint16_t sample = 0; sample < CONTROLLER_BUFF_SAMPLES; sample++) {
    primary_buff[sample].temperature = 0;
    primary_buff[sample].humidity = INT16_MAX;
    outside_buff[sample].temperature = 0;
    outside_buff[sample].humidity = INT16_MAX;
  }

  TimerSet(&fridge_off_timer, 1);

  if (config->flags & CONFIG_FLAG_AUTOSTART) {
    ControllerEnable(true);
  }

  return rval;
}

void ControllerControl(th_value_t *values) {
  int32_t primary_t_avg = 0;
  int32_t outside_t_avg = 0;
  static bool enable_state = false;

  // Update value buffer with latest data
  primary_buff[buff_index].temperature =
      values[config->primary_sensor].temperature;
  primary_buff[buff_index].humidity = values[config->primary_sensor].humidity;
  outside_buff[buff_index].temperature =
      values[config->outside_sensor].temperature;
  outside_buff[buff_index].humidity = values[config->outside_sensor].humidity;

  buff_index = (buff_index + 1) & (CONTROLLER_BUFF_SAMPLES - 1);

  // Average temperatures
  for (uint16_t sample = 0; sample < CONTROLLER_BUFF_SAMPLES; sample++) {
    primary_t_avg += primary_buff[sample].temperature;
    outside_t_avg += outside_buff[sample].temperature;
  }

  primary_t_avg /= CONTROLLER_BUFF_SAMPLES;
  outside_t_avg /= CONTROLLER_BUFF_SAMPLES;

  if ((primary_t_avg > config->temp_set) &&
      (config->temp_set < outside_t_avg)) {
    if (TimerExpired(&fridge_off_timer)) {
      GPIO_SetBits(FRIDGE_PORT, (1 << FRIDGE_PIN));
      GPIO_SetBits(LED2_PORT, (1 << LED2_PIN));
      enable_state = true;
    }
  } else {
    GPIO_ResetBits(FRIDGE_PORT, (1 << FRIDGE_PIN));
    GPIO_ResetBits(LED2_PORT, (1 << LED2_PIN));

    // Only clear timer when changing states
    if (enable_state == true) {
      TimerSet(&fridge_off_timer, FRIDGE_OFF_TIME_M * 60 * 1000);
    }
    enable_state = false;
  }
}

void ControllerProcess() {
  int32_t rval;
  th_value_t values[CONFIG_MAX_SENSORS];

  if (!running) {
    return;
  }

  if (TimerExpired(&controller_timer)) {
    GPIO_SetBits(LED1_PORT, (1 << LED1_PIN));

    TimerSet(&controller_timer, config->period_ms);

    for (uint8_t sensor_id = 0; sensor_id < CONFIG_MAX_SENSORS; sensor_id++) {
      th_sensor_t *sensor = &config->sensor[sensor_id];
      if (sensor->addr != 0) {
        rval = Tca95xxaSetChannel(TCA95XXA_ADDR, sensor->bus);
        if (rval != 0) {
          dprint(ERR, "could not set i2c bus (%ld)\n", rval);
          GPIO_ResetBits(TCA_nRST_PORT, (1 << TCA_nRST_PIN));
          SleepMs(2);
          GPIO_SetBits(TCA_nRST_PORT, (1 << TCA_nRST_PIN));
          i2cSetup(100000);
          rval = Tca95xxaSetChannel(TCA95XXA_ADDR, sensor->bus);
          dprint(ERR, "Retry %d\n", rval);
          if (rval != 0) {
            break;
          }
          break;
        }

        rval = Sht31Read(sensor->addr, &values[sensor_id].temperature,
                          &values[sensor_id].humidity);
        if (rval != 0) {
          dprint(ERR, "SHT could not read temp/humidity for sensor%d (%ld)\n",
                 sensor_id, rval);
          i2cSetup(100000);  // Attempt to recover from I2C error
          break;
        }
      }
    }

    ControllerControl(values);

    dprint(DATA, "%s,", RtcGetTimeStr());

    for (uint8_t sensor_id = 0; sensor_id < CONFIG_MAX_SENSORS; sensor_id++) {
      th_sensor_t *sensor = &config->sensor[sensor_id];
      if (sensor->addr != 0) {
        int16_t temperature = values[sensor_id].temperature;
        int16_t humidity = values[sensor_id].humidity;
        char *sign = "";
        if (temperature < 0) {
          sign = "-";
          temperature *= -1;
        }
        dprint(OK_CONT, "%s%d.%02d,", sign, temperature / 100,
               (temperature - (temperature / 100) * 100));
        dprint(OK_CONT, "%d.%02d,", humidity / 100,
               (humidity - (humidity / 100) * 100));
      }
    }
    dprint(OK_CONT, "\n");

    GPIO_ResetBits(LED1_PORT, (1 << LED1_PIN));
  }
}

int32_t ControllerEnable(bool enabled) {
  if (enabled && !running) {
    config_t *config = ConfigGet();
    dprint(DATA, "timestamp,");
    for (uint8_t sensor_id = 0; sensor_id < CONFIG_MAX_SENSORS; sensor_id++) {
      th_sensor_t *sensor = &config->sensor[sensor_id];
      if (sensor->addr != 0) {
        dprint(OK_CONT, "sensor%d_t,sensor%d_h,", sensor_id, sensor_id);
      }
    }
    dprint(OK_CONT, "\n");

    TimerSet(&controller_timer, 1);
  } else if (!enabled) {
    TimerClear(&controller_timer);
  }

  running = enabled;

  return 0;
}

int32_t ControllerAutoconfig() {
  int32_t rval;
  uint8_t addresses[] = {SHT31_ADDR, SHT31_ALT_ADDR};
  uint8_t sensor_id = 0;
  config_t *config = ConfigGet();

  // Clear current sensors
  memset(&config->sensor, 0, sizeof(th_sensor_t) * CONFIG_MAX_SENSORS);

  for (uint8_t bus = 0; bus < TCA95XXA_CHANNELS; bus++) {
    for (uint8_t addr = 0; addr < sizeof(addresses) / sizeof(uint8_t); addr++) {
      if (sensor_id > CONFIG_MAX_SENSORS) {
        dprint(ERR, "maximum number of devices reached\n");
        break;
      }

      rval = Tca95xxaSetChannel(TCA95XXA_ADDR, bus);
      if (rval != 0) {
        dprint(ERR, "could not set i2c bus (%ld)\n", rval);
        GPIO_ResetBits(TCA_nRST_PORT, (1 << TCA_nRST_PIN));
        SleepMs(2);
        GPIO_SetBits(TCA_nRST_PORT, (1 << TCA_nRST_PIN));
        i2cSetup(100000);
        rval = Tca95xxaSetChannel(TCA95XXA_ADDR, bus);
        dprint(ERR, "Retry %d\n", rval);
        if (rval != 0) {
          break;
        }
        break;
      }

      rval = Sht31Init(addresses[addr]);
      if (rval == 0) {
        dprint(OK, "found sensor with addr %02X on bus %d\n", addresses[addr],
               bus);
        config->sensor[sensor_id].addr = addresses[addr];
        config->sensor[sensor_id].bus = bus;
        sensor_id++;
      }
    }
  }

  ConfigWrite();
  dprint(OK, "config updated\n");

  return 0;
}
