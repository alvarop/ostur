#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "fifo.h"
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"

typedef struct {
	char *commandStr;
	void (*fn)(uint8_t argc, char *argv[]);
	char *helpStr;
} command_t;

extern fifo_t rxFifo;

static uint8_t *uid = (uint8_t *)(0x1FFFF7AC);

static char cmdBuff[64];
static uint8_t argc;
static char* argv[255];

static void helpFn(uint8_t argc, char *argv[]);
static void snCmd(uint8_t argc, char *argv[]);
static void versionCmd(uint8_t argc, char *argv[]);

static const char versionStr[] = FW_VERSION;

static command_t commands[] = {
	{"sn", snCmd, "sn"},
	{"version", versionCmd, "version"},
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
			printf("%s - %s\n", command->commandStr, command->helpStr);
			command++;
		}
	} else {
		while(command->commandStr != NULL) {
			if(strcmp(command->commandStr, argv[1]) == 0) {
				printf("%s - %s\n", command->commandStr, command->helpStr);
				break;
			}
			command++;
		}
	}
}

static void snCmd(uint8_t argc, char *argv[]) {
	printf("OK ");

	// Print 96-bit serial number
	for(uint8_t byte = 0; byte < 12; byte++) {
		printf("%02X ", uid[byte]);
	}
	printf("\n");
}

static void versionCmd(uint8_t argc, char *argv[]) {
	printf("OK %s\n", versionStr);
}

void consoleProcess() {
	uint32_t inBytes = fifoSize(&rxFifo);
	if(inBytes > 0) {
		uint32_t newLine = 0;
		for(int32_t index = 0; index < inBytes; index++){
			if((fifoPeek(&rxFifo, index) == '\n') || (fifoPeek(&rxFifo, index) == '\r')) {
				newLine = index + 1;
				GPIO_SetBits(GPIOB, GPIO_Pin_2);
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
					printf("ERR Unknown command '%s'\n", argv[0]);
				}
			}
		}
	}
}