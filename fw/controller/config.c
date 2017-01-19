#include <stdio.h>
#include "config.h"
#include "stm32f0xx_flash.h"
#include "debug.h"

#define CONFIG_ADDR 	(0x0801F800)
#define CONFIG_MAX_SIZE	(0x800)
#define CONFIG_MAGIC	(0xDEADC0DE)

static config_t *config = (config_t *)CONFIG_ADDR;

int32_t config_init() {
	// TODO: Use CRC

	if(sizeof(config_t) > CONFIG_MAX_SIZE) {
		dprint(ERR, "PANIC! config is too large\n");
		while(1);
	}

	if(config->magic != CONFIG_MAGIC) {
		config_t default_config = {
			.magic = CONFIG_MAGIC,
			.period_ms = 2000,
			.temp_set = 1000,
			.humid_set = 7500,
			.primary_sensor = 0,
			.outside_sensor = 1,
			.sensor = {
				{0x88, 0},
				{0x88, 1},
				{0x88, 2},
				{0x88, 5},
				{0x88, 6},
				{0x88, 7},
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
		dprint(ERR, "Config not available.\n Writing defaults\n");

		config_write(&default_config);
	} else {
		dprint(INFO, "Configuration loaded\n");
	}

	return 0;
}

config_t *config_get() {
	// TODO - add magic/crc check and return NULL if invalid
	return config;
}

int32_t config_write(config_t *new_config) {
	uint16_t *buff = (uint16_t *)new_config;

	// TODO - compute new CRC

	dprint(INFO, "Writing new configuration...\n");

	FLASH_Unlock();
	FLASH_ErasePage(CONFIG_ADDR);

	// We can only write two bytes at a time
	for(uint32_t halfword = 0; halfword < sizeof(config_t)/2; halfword++) {
		FLASH_ProgramHalfWord(CONFIG_ADDR + halfword * 2, buff[halfword]);
	}

	FLASH_Lock();

	config_print(config);
	return 0;
}

int32_t config_print(config_t *config_to_print) {
	dprint(INFO, "Device Configuration:\n");
	dprint(INFO, "magic: %08lX\n", config_to_print->magic);
	dprint(INFO, "period_ms: %ld\n", config_to_print->period_ms);
	dprint(INFO, "temp_set: %d\n", config_to_print->temp_set);
	dprint(INFO, "humid_set: %d\n", config_to_print->humid_set);
	dprint(INFO, "primary_sensor: %d\n", config_to_print->primary_sensor);
	dprint(INFO, "outside_sensor: %d\n", config_to_print->outside_sensor);

	dprint(INFO, "Sensors:\n");
	for(uint8_t sensor_id = 0; sensor_id < CONFIG_MAX_SENSORS; sensor_id++) {
		th_sensor_t *sensor = &config_to_print->sensor[sensor_id];
		dprint(INFO, "  sensor %d - addr: %02X bus: %d\n",
			sensor_id, sensor->addr, sensor->bus);
	}

	dprint(INFO, "crc16: %04X\n", config_to_print->crc16);

	return 0;
}
