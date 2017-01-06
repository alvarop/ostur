#include <stdio.h>
#include "controller.h"
#include "timer.h"
#include "config.h"
#include "sht31.h"
#include "tca9584a.h"

static bool running = false;
static config_t *config;
static ms_timer_t controller_timer;

int32_t controller_init() {
	int32_t rval;
	config = config_get();
	printf("%s\n", __func__);
	for(uint8_t sensor_id = 0; sensor_id < CONFIG_MAX_SENSORS; sensor_id++) {
		th_sensor_t *sensor = &config->sensor[sensor_id];
		if(sensor->addr != 0) {
			rval = tca9584a_set_channel(TCA9548A_ADDR, sensor->bus);
			if(rval != 0) {
				printf("ERR: could not set i2c bus (%ld)\n", rval);
				break;
			}

			sht31_init(sensor->addr);
			if(rval != 0) {
				printf("ERR: SHT could not initialize (%ld)\n", rval);
				break;
			}
		}
	}
	printf("\n");

	config_print();

	return rval;
}

void controller_process() {
	int32_t rval;
	if(!running) {
		return;
	}

	if(timer_expired(&controller_timer)) {
		// TODO - measure all before printing
		// TODO - average and control fridge
		timer_set(&controller_timer, config->period_ms);

		printf("%ld,", get_tick_ms());

		for(uint8_t sensor_id = 0; sensor_id < CONFIG_MAX_SENSORS; sensor_id++) {
			th_sensor_t *sensor = &config->sensor[sensor_id];
			if(sensor->addr != 0) {
				int16_t temperature = 0;
				int16_t humidity = 0;

				rval = tca9584a_set_channel(TCA9548A_ADDR, sensor->bus);
				if(rval != 0) {
					printf("ERR: could not set i2c bus (%ld)\n", rval);
					break;
				}

				rval = sht31_read(SHT31_ADDR, &temperature, &humidity);
				if(rval != 0) {
					printf("ERR: SHT could not read temp/humidity (%ld)\n", rval);
					break;
				}
				printf("%d.%02d,", temperature/100, (temperature-(temperature/100) * 100));
				printf("%d.%02d,", humidity/100, (humidity-(humidity/100) * 100));
			}
		}
		printf("\n");
	}
}

int32_t controller_enable(bool enabled) {
	if(enabled && !running){
		config_t *config = config_get();
		printf("timestamp,");
		for(uint8_t sensor_id = 0; sensor_id < CONFIG_MAX_SENSORS; sensor_id++) {
			th_sensor_t *sensor = &config->sensor[sensor_id];
			if(sensor->addr != 0) {
				printf("sensor%d_t,sensor%d_h,",sensor_id,sensor_id);
			}
		}
		printf("\n");

		timer_set(&controller_timer, 1);
	} else {
		timer_clear(&controller_timer);
	}

	running = enabled;

	return 0;
}
