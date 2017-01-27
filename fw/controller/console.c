#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "i2c.h"
#include "fifo.h"
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "tca9584a.h"
#include "sht31.h"
#include "timer.h"
#include "controller.h"
#include "debug.h"
#include "rtc.h"

typedef struct {
	char *commandStr;
	void (*fn)(uint8_t argc, char *argv[]);
	char *helpStr;
} command_t;

extern fifo_t rxFifo;

static uint8_t *uid = (uint8_t *)(0x1FFFF7AC);

static char cmdBuff[1024];
static uint8_t argc;
static char* argv[255];

static void helpFn(uint8_t argc, char *argv[]);
static void i2cCmd(uint8_t argc, char *argv[]);
static void shtCmd(uint8_t argc, char *argv[]);
static void snCmd(uint8_t argc, char *argv[]);
static void controllerCmd(uint8_t argc, char *argv[]);
static void versionCmd(uint8_t argc, char *argv[]);
static void timeCmd(uint8_t argc, char *argv[]);
static void resetCmd(uint8_t argc, char *argv[]);

static const char versionStr[] = FW_VERSION;

static command_t commands[] = {
	{"i2c", i2cCmd, "i2c"},
	{"sht", shtCmd, "sht31 stuff"},
	{"sn", snCmd, "sn"},
	{"controller", controllerCmd, "controller <start|stop|autoconfig>"},
	{"version", versionCmd, "version"},
	{"reset", resetCmd, "System reset"},
	{"time", timeCmd, "time <YYYY MM DD HH MM SS>"},
	// Add new commands here!
	{"help", helpFn, "Print this!"},
	{NULL, NULL, NULL}
};

//
// Print the help menu
//
static void helpFn(uint8_t argc, char *argv[]) {
	command_t *command = commands;

	if(argc < 2) {
		while(command->commandStr != NULL) {
			dprint(INFO, "%s - %s\n", command->commandStr, command->helpStr);
			command++;
		}
	} else {
		while(command->commandStr != NULL) {
			if(strcmp(command->commandStr, argv[1]) == 0) {
				dprint(INFO, "%s - %s\n", command->commandStr, command->helpStr);
				break;
			}
			command++;
		}
	}
}

#define I2C_ADDR_OFFSET		(1)
#define I2C_RLEN_OFFSET		(2)
#define I2C_WBUFF_OFFSET	(3)
static void i2cCmd(uint8_t argc, char *argv[]) {
	uint8_t wBuff[128];
	uint8_t rBuff[128];
	int32_t rval;

	do {
		if(argc < 3) {
			dprint(ERR, "I2C Not enough arguments\n");
			break;
		}

		uint8_t addr = strtoul(argv[I2C_ADDR_OFFSET], NULL, 16);
		uint8_t rLen = strtoul(argv[I2C_RLEN_OFFSET], NULL, 10);
		uint8_t wLen = argc - I2C_WBUFF_OFFSET;

		if(wLen > sizeof(wBuff)) {
			dprint(ERR, "I2C Not enough space in wBuff\n");
			break;
		}

		if(rLen > sizeof(rBuff)) {
			dprint(ERR, "I2C Not enough space in rBuff\n");
			break;
		}

		if(wLen > 0) {
			for(uint32_t byte = 0; byte < wLen; byte++) {
				wBuff[byte] = strtoul(argv[I2C_WBUFF_OFFSET + byte], NULL, 16);
			}
		}

		rval = i2c(I2C1, addr, wLen, wBuff, rLen, rBuff);

		if(rval) {
			dprint(ERR, "%ld\n", rval);
		} else {
			dprint(OK, "");
			for(uint32_t byte = 0; byte < rLen; byte++) {
				dprint(OK_CONT, "%02X ", rBuff[byte]);
			}
			dprint(OK_CONT, "\n");
		}

	} while (0);
}

static void timeCmd(uint8_t argc, char *argv[]) {
	do {

		if(argc == 1) {
			dprint(OK, "%s\n", rtc_get_time_str());
		} else if (argc == 7) {
			uint16_t year = strtoul(argv[1], NULL, 10);
			uint16_t month = strtoul(argv[2], NULL, 10);
			uint16_t day = strtoul(argv[3], NULL, 10);
			uint16_t hour = strtoul(argv[4], NULL, 10);
			uint16_t minute = strtoul(argv[5], NULL, 10);
			uint16_t second = strtoul(argv[6], NULL, 10);

			rtc_set_time(year, month, day, hour, minute, second);
			dprint(OK, "%s\n", rtc_get_time_str());
		} else {
			dprint(ERR, "invalid arguments argc=%d\n", argc);
		}

	} while(0);
}

static void shtCmd(uint8_t argc, char *argv[]) {
	int32_t rval;

	do {
		if(argc < 3) {
			dprint(ERR, "sht <init|read> <channel>\n");
			break;
		}

		uint8_t ch = strtoul(argv[2], NULL, 10);
		rval = tca9584a_set_channel(TCA9548A_ADDR, ch);

		if(rval != 0) {
			dprint(ERR, "SHT could not set channel\n");
			break;
		}

		if(strcmp("init", argv[1]) == 0) {
			rval = sht31_init(SHT31_ADDR);
			if(rval == 0) {
				dprint(OK, "\n");
			}
		} else if (strcmp("read", argv[1]) == 0) {
			int16_t temperature = 0;
			int16_t humidity = 0;
			rval = sht31_read(SHT31_ADDR, &temperature, &humidity);
			if(rval != 0) {
				dprint(ERR, "SHT could not read temp/humidity\n");
				break;
			}
			dprint(OK, " %d.%02d ", temperature/100, (temperature-(temperature/100) * 100));
			dprint(OK_CONT, "%d.%02d\n", humidity/100, (humidity-(humidity/100) * 100));
		}

	} while(0);
}

static void controllerCmd(uint8_t argc, char *argv[]) {
	do {
		if(argc < 2) {
			dprint(ERR, "controller <start|stop|autoconfig>\n");
			break;
		}

		if(strcmp("start", argv[1]) == 0) {
			controller_enable(true);
		} else if (strcmp("stop", argv[1]) == 0) {
			controller_enable(false);
			dprint(OK, "Controller stopped\n");
		} else if (strcmp("autoconfig", argv[1]) == 0) {
			controller_autoconfig();
		} else {
			dprint(ERR, "controller <start|stop|autoconfig>\n");
		}

	} while(0);
}

static void snCmd(uint8_t argc, char *argv[]) {
	dprint(OK, "");

	// Print 96-bit serial number
	for(uint8_t byte = 0; byte < 12; byte++) {
		dprint(OK_CONT, "%02X ", uid[byte]);
	}
	dprint(OK_CONT, "\n");
}

static void versionCmd(uint8_t argc, char *argv[]) {
	dprint(OK, "%s\n", versionStr);
}

static void resetCmd(uint8_t argc, char *argv[]) {
	NVIC_SystemReset();
}

void consoleProcess() {
	uint32_t inBytes = fifoSize(&rxFifo);
	if(inBytes > 0) {
		uint32_t newLine = 0;
		for(int32_t index = 0; index < inBytes; index++){
			if((fifoPeek(&rxFifo, index) == '\n') || (fifoPeek(&rxFifo, index) == '\r')) {
				newLine = index + 1;
				break;
			}
		}

		if(newLine > sizeof(cmdBuff)) {
			newLine = sizeof(cmdBuff) - 1;
		}

		if(newLine) {
			uint8_t *pBuf = (uint8_t *)cmdBuff;
			while(newLine--){
				*pBuf++ = fifoPop(&rxFifo);
			}

			// If it's an \r\n combination, discard the second one
			if((fifoPeek(&rxFifo, 0) == '\n') || (fifoPeek(&rxFifo, 0) == '\r')) {
				fifoPop(&rxFifo);
			}

			*(pBuf - 1) = 0; // String terminator

			argc = 0;

			// Get command
			argv[argc] = strtok(cmdBuff, " ");

			// Get arguments (if any)
			while ((argc < sizeof(argv)/sizeof(char *)) && (argv[argc] != NULL)){
				argc++;
				argv[argc] = strtok(NULL, " ");
			}

			if(argc > 0) {
				command_t *command = commands;
				while(command->commandStr != NULL) {
					if(strcmp(command->commandStr, argv[0]) == 0) {
						command->fn(argc, argv);
						break;
					}
					command++;
				}

				if(command->commandStr == NULL) {
					dprint(ERR, "Unknown command '%s'\n", argv[0]);
				}
			}
		}
	}
}
