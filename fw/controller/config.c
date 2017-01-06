#include <stdio.h>
#include "config.h"

#define CONFIG_ADDR 	(0x0801F800)
#define CONFIG_MAX_SIZE	(0x800)
#define CONFIG_MAGIC	(0xDEADC0DE)

static config_t *config = (config_t *)CONFIG_ADDR;

int32_t config_init() {
	// TODO: Use CRC

	if(sizeof(config_t) > CONFIG_MAX_SIZE) {
		printf("PANIC! config is too large\n");
		while(1);
	}

	if(config->magic != CONFIG_MAGIC) {
		config_t default_config = {
			.magic = CONFIG_MAGIC,
			.period_ms = 5000,
			.temp_set = 1500,
			.humid_set = 7500,
			.sensor = {
				{0x88, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
				{0x00, 0},
			},
			.crc16 = 0x0000
		};
		printf("ERR Config not available.\n Writing defaults\n");

		config_write(&default_config);
	} else {
		printf("Configuration loaded\n");
	}

	return 0;
}

config_t *config_get() {
	return config;
}

int32_t config_write(config_t *new_config) {


	config_print(new_config);
	return 0;
}

int32_t config_print() {
	printf("OK Device Configuration:\n");
	printf("magic: %08lX\n", config->magic);
	printf("period_ms: %ld\n", config->period_ms);
	printf("temp_set: %d\n", config->temp_set);
	printf("humid_set: %d\n", config->humid_set);

	printf("Sensors:\n");
	for(uint8_t sensor_id = 0; sensor_id < MAX_SENSORS; sensor_id++) {
		th_sensor_t *sensor = &config->sensor[sensor_id];
		printf("  sensor %d - addr: %02X bus: %d\n",
			sensor_id, sensor->addr, sensor->bus);
	}

	printf("crc16: %04X\n", config->crc16);

	return 0;
}
