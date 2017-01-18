#include <stdio.h>
#include <string.h>
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

	config_print(config_get());

	return rval;
}

void controller_process() {
	int32_t rval;
	int16_t values[CONFIG_MAX_SENSORS][2];
	uint32_t timestamp;

	if(!running) {
		return;
	}

	if(timer_expired(&controller_timer)) {
		// TODO - average and control fridge
		timer_set(&controller_timer, config->period_ms);
		timestamp = get_tick_ms();

		for(uint8_t sensor_id = 0; sensor_id < CONFIG_MAX_SENSORS; sensor_id++) {
			th_sensor_t *sensor = &config->sensor[sensor_id];
			if(sensor->addr != 0) {
				rval = tca9584a_set_channel(TCA9548A_ADDR, sensor->bus);
				if(rval != 0) {
					printf("ERR: could not set i2c bus (%ld)\n", rval);
					break;
				}

				rval = sht31_read(SHT31_ADDR,
									&values[sensor_id][0],
									&values[sensor_id][1]);
				if(rval != 0) {
					printf("ERR: SHT could not read temp/humidity (%ld)\n", rval);
					break;
				}
			}
		}

		printf("%ld,", timestamp);

		for(uint8_t sensor_id = 0; sensor_id < CONFIG_MAX_SENSORS; sensor_id++) {
			th_sensor_t *sensor = &config->sensor[sensor_id];
			if(sensor->addr != 0) {
				int16_t temperature = values[sensor_id][0];
				int16_t humidity = values[sensor_id][1];
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

int32_t controller_autoconfig() {
	int32_t rval;
	uint8_t addresses[] = {SHT31_ADDR, SHT31_ALT_ADDR};
	uint8_t sensor_id = 0;
	config_t *current_config = config_get();
	config_t new_config;

	// Clone current config
	memcpy(&new_config, current_config, sizeof(config_t));

	// Clear current sensors
	memset(&new_config.sensor, 0, sizeof(th_sensor_t) * CONFIG_MAX_SENSORS);

	for(uint8_t bus = 0; bus < TCA9548A_CHANNELS; bus++) {
		for(uint8_t addr = 0; addr < sizeof(addresses)/sizeof(uint8_t); addr++) {

			if(sensor_id > CONFIG_MAX_SENSORS) {
				printf("ERR: maximum number of devices reached\n");
				break;
			}

			rval = tca9584a_set_channel(TCA9548A_ADDR, bus);
			if(rval != 0) {
				printf("ERR: could not set i2c bus (%ld)\n", rval);
				break;
			}

			rval = sht31_init(addresses[addr]);
			if (rval == 0) {
				printf("OK found sensor with addr %02X on bus %d\n", addresses[addr], bus);
				new_config.sensor[sensor_id].addr = addresses[addr];
				new_config.sensor[sensor_id].bus = bus;
				sensor_id++;
			}
		}
	}

	if(config_write(&new_config)) {
		printf("OK config updated\n");
	}

	return 0;
}
