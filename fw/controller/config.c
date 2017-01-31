#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"
#include "stm32f0xx_flash.h"
#include "debug.h"

#define CONFIG_ADDR 	(0x0801F800)
#define CONFIG_MAX_SIZE	(0x800)
#define CONFIG_MAGIC	(0xDEADC0DE)

static config_t *flash_config = (config_t *)CONFIG_ADDR;
static config_t config;

int32_t config_init() {
	// TODO: Use CRC

	if(sizeof(config_t) > CONFIG_MAX_SIZE) {
		dprint(ERR, "PANIC! config is too large\n");
		while(1);
	}

	// Copy flash config to RAM
	memcpy(&config, flash_config, sizeof(config_t));

	if(config.magic != CONFIG_MAGIC) {
		config_t default_config = {
			.magic = CONFIG_MAGIC,
			.period_ms = 5000,
			.temp_set = 1000,
			.humid_set = 7500,
			.primary_sensor = 0,
			.outside_sensor = 1,
			.flags = CONFIG_FLAG_AUTOSTART,
			.sensor = {
				{0x88, 0},
				{0x88, 1},
				{0x88, 0},
				{0x88, 0},
				{0x88, 0},
				{0x88, 0},
				{0x00, 0},
				{0x00, 0},
			},
			.crc16 = 0x0000
		};

		memcpy(&config, &default_config, sizeof(config_t));

		dprint(ERR, "Config not available.\n Writing defaults\n");

		config_write();
	} else {
		dprint(INFO, "Configuration loaded\n");
	}

	return 0;
}

config_t *config_get() {
	// TODO - add magic/crc check and return NULL if invalid
	return &config;
}

// Write current configuration to flash
void config_write() {
	uint16_t *buff = (uint16_t *)&config;

	// TODO - compute new CRC

	dprint(INFO, "Writing new configuration...\n");

	FLASH_Unlock();
	FLASH_ErasePage(CONFIG_ADDR);

	// We can only write two bytes at a time
	for(uint32_t halfword = 0; halfword < sizeof(config_t)/2; halfword++) {
		FLASH_ProgramHalfWord(CONFIG_ADDR + halfword * 2, buff[halfword]);
	}

	FLASH_Lock();

}

void config_print() {
	dprint(INFO, "Device Configuration:\n");
	dprint(INFO, "magic: %08lX\n", config.magic);
	dprint(INFO, "period_ms: %ld\n", config.period_ms);
	dprint(INFO, "temp_set: %d\n", config.temp_set);
	dprint(INFO, "humid_set: %d\n", config.humid_set);
	dprint(INFO, "primary_sensor: %d\n", config.primary_sensor);
	dprint(INFO, "outside_sensor: %d\n", config.outside_sensor);
	dprint(INFO, "flags: %04X\n", config.flags);

	dprint(INFO, "Sensors:\n");
	for(uint8_t sensor_id = 0; sensor_id < CONFIG_MAX_SENSORS; sensor_id++) {
		th_sensor_t *sensor = &config.sensor[sensor_id];
		dprint(INFO, "  sensor %d - addr: %02X bus: %d\n",
			sensor_id, sensor->addr, sensor->bus);
	}

	dprint(INFO, "crc16: %04X\n", config.crc16);
}


typedef struct {
	char *key;
	void (*get)();
	void (*set)(uint8_t argc, char *argv[]);
} command_t;

void _get_period() {
	dprint(OK, "%d\n", config.period_ms);
}

void _set_period(uint8_t argc, char *argv[]) {
	config.period_ms = strtoul(argv[2], NULL, 10);
	dprint(OK, "\n");
}

void _get_temp() {
	dprint(OK, "%d\n", config.temp_set);
}

void _set_temp(uint8_t argc, char *argv[]) {
	config.temp_set = strtoul(argv[2], NULL, 10);
	dprint(OK, "\n");
}

void _get_p_sensor() {
	dprint(OK, "%d\n", config.primary_sensor);
}

void _set_p_sensor(uint8_t argc, char *argv[]) {
	config.primary_sensor = strtoul(argv[2], NULL, 10);
	dprint(OK, "\n");
}

void _get_o_sensor() {
	dprint(OK, "%d\n", config.outside_sensor);
}

void _set_o_sensor(uint8_t argc, char *argv[]) {
	config.outside_sensor = strtoul(argv[2], NULL, 10);
	dprint(OK, "\n");
}

static command_t commands[] = {
	{"write", config_write, NULL},
	{"print", config_print, NULL},
	{"period", _get_period, _set_period},
	{"temp", _get_temp, _set_temp},
	{"psensor", _get_p_sensor, _set_p_sensor},
	{"osensor", _get_o_sensor, _set_o_sensor},
	{NULL, NULL, NULL}
};


void config_cmd(uint8_t argc, char *argv[]) {
	do {
		if(argc < 2) {
			dprint(ERR, "config key [value]\n");
			break;
		}

		command_t *command = commands;
		while(command->key != NULL) {
			if(strcmp(command->key, argv[1]) == 0) {
				if(argc == 2) {
					command->get();
				} else {
					if(command->set != NULL){
						command->set(argc, argv);
					}
				}
				break;
			}
			command++;
		}

		if(command->key == NULL) {
			dprint(ERR, "Unknown key '%s'\n", argv[1]);
		}




	} while(0);
}
