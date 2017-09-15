#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "config.h"
#include "controller.h"
#include "debug.h"
#include "fifo.h"
#include "i2c.h"
#include "rtc.h"
#include "sht31.h"
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "tca95xxa.h"
#include "timer.h"

typedef struct {
  char *commandStr;
  void (*fn)(uint8_t argc, char *argv[]);
  char *helpStr;
} command_t;

extern fifo_t rxFifo;

static uint8_t *uid = (uint8_t *)(0x1FFFF7AC);

static char cmdBuff[1024];
static uint8_t argc;
static char *argv[255];

static void HelpFn(uint8_t argc, char *argv[]);
static void I2cCmd(uint8_t argc, char *argv[]);
static void ShtCmd(uint8_t argc, char *argv[]);
static void SnCmd(uint8_t argc, char *argv[]);
static void ControllerCmd(uint8_t argc, char *argv[]);
static void VersionCmd(uint8_t argc, char *argv[]);
static void TimeCmd(uint8_t argc, char *argv[]);
static void ResetCmd(uint8_t argc, char *argv[]);

static const char versionStr[] = FW_VERSION;

static command_t commands[] = {
    {"i2c", I2cCmd, "i2c"},
    {"sht", ShtCmd, "sht31 stuff"},
    {"sn", SnCmd, "sn"},
    {"controller", ControllerCmd, "controller <start|stop|autoconfig>"},
    {"version", VersionCmd, "version"},
    {"reset", ResetCmd, "System reset"},
    {"time", TimeCmd, "time <YYYY MM DD HH MM SS>"},
    {"config", ConfigCmd, "config key [value]"},
    // Add new commands here!
    {"help", HelpFn, "Print this!"},
    {NULL, NULL, NULL}};

//
// Print the help menu
//
static void HelpFn(uint8_t argc, char *argv[]) {
  command_t *command = commands;

  if (argc < 2) {
    while (command->commandStr != NULL) {
      dprint(INFO, "%s - %s\n", command->commandStr, command->helpStr);
      command++;
    }
  } else {
    while (command->commandStr != NULL) {
      if (strcmp(command->commandStr, argv[1]) == 0) {
        dprint(INFO, "%s - %s\n", command->commandStr, command->helpStr);
        break;
      }
      command++;
    }
  }
}

#define I2C_ADDR_OFFSET (1)
#define I2C_RLEN_OFFSET (2)
#define I2C_WBUFF_OFFSET (3)
static void I2cCmd(uint8_t argc, char *argv[]) {
  uint8_t wBuff[128];
  uint8_t rBuff[128];
  int32_t rval;

  do {
    if (argc < 3) {
      dprint(ERR, "I2C Not enough arguments\n");
      break;
    }

    uint8_t addr = strtoul(argv[I2C_ADDR_OFFSET], NULL, 16);
    uint8_t rLen = strtoul(argv[I2C_RLEN_OFFSET], NULL, 10);
    uint8_t wLen = argc - I2C_WBUFF_OFFSET;

    if (wLen > sizeof(wBuff)) {
      dprint(ERR, "I2C Not enough space in wBuff\n");
      break;
    }

    if (rLen > sizeof(rBuff)) {
      dprint(ERR, "I2C Not enough space in rBuff\n");
      break;
    }

    if (wLen > 0) {
      for (uint32_t byte = 0; byte < wLen; byte++) {
        wBuff[byte] = strtoul(argv[I2C_WBUFF_OFFSET + byte], NULL, 16);
      }
    }

    rval = i2c(I2C1, addr, wLen, wBuff, rLen, rBuff);

    if (rval) {
      dprint(ERR, "%ld\n", rval);
    } else {
      dprint(OK, "");
      for (uint32_t byte = 0; byte < rLen; byte++) {
        dprint(OK_CONT, "%02X ", rBuff[byte]);
      }
      dprint(OK_CONT, "\n");
    }

  } while (0);
}

static void TimeCmd(uint8_t argc, char *argv[]) {
  do {
    if (argc == 1) {
      dprint(OK, "%s\n", RtcGetTimeStr());
    } else if (argc == 7) {
      uint16_t year = strtoul(argv[1], NULL, 10);
      uint16_t month = strtoul(argv[2], NULL, 10);
      uint16_t day = strtoul(argv[3], NULL, 10);
      uint16_t hour = strtoul(argv[4], NULL, 10);
      uint16_t minute = strtoul(argv[5], NULL, 10);
      uint16_t second = strtoul(argv[6], NULL, 10);

      RtcSetTime(year, month, day, hour, minute, second);
      dprint(OK, "%s\n", RtcGetTimeStr());
    } else {
      dprint(ERR, "invalid arguments argc=%d\n", argc);
    }

  } while (0);
}

static void ShtCmd(uint8_t argc, char *argv[]) {
  int32_t rval;

  do {
    if (argc < 3) {
      dprint(ERR, "sht <init|read|h_on|h_off> <channel>\n");
      break;
    }

    uint8_t ch = strtoul(argv[2], NULL, 10);
    rval = Tca95xxaSetChannel(TCA95XXA_ADDR, ch);

    if (rval != 0) {
      dprint(ERR, "SHT could not set channel\n");
      GPIO_ResetBits(TCA_nRST_PORT, (1 << TCA_nRST_PIN));
      SleepMs(2);
      GPIO_SetBits(TCA_nRST_PORT, (1 << TCA_nRST_PIN));
      i2cSetup(100000);
      rval = Tca95xxaSetChannel(TCA95XXA_ADDR, ch);
      dprint(ERR, "Retry %d\n", rval);
      if (rval != 0) {
        break;
      }
      break;
    }

    if (strcmp("init", argv[1]) == 0) {
      rval = Sht31Init(SHT31_ADDR);
      if (rval == 0) {
        dprint(OK, "\n");
      }
    } else if (strcmp("read", argv[1]) == 0) {
      int16_t temperature = 0;
      int16_t humidity = 0;
      rval = Sht31Read(SHT31_ADDR, &temperature, &humidity);
      if (rval != 0) {
        dprint(ERR, "SHT could not read temp/humidity\n");
        break;
      }
      dprint(OK, " %d.%02d ", temperature / 100,
             (temperature - (temperature / 100) * 100));
      dprint(OK_CONT, "%d.%02d\n", humidity / 100,
             (humidity - (humidity / 100) * 100));
    } else if (strcmp("h_on", argv[1]) == 0) {
      rval = Sht31Heater(SHT31_ADDR, true);
      if (rval != 0) {
        dprint(ERR, "SHT could not enable heater\n");
        break;
      }
    } else if (strcmp("h_off", argv[1]) == 0) {
      rval = Sht31Heater(SHT31_ADDR, false);
      if (rval != 0) {
        dprint(ERR, "SHT could not enable heater\n");
        break;
      }
    }

  } while (0);
}

static void ControllerCmd(uint8_t argc, char *argv[]) {
  do {
    if (argc < 2) {
      dprint(ERR, "controller <start|stop|autoconfig>\n");
      break;
    }

    if (strcmp("start", argv[1]) == 0) {
      ControllerEnable(true);
    } else if (strcmp("stop", argv[1]) == 0) {
      ControllerEnable(false);
      dprint(OK, "Controller stopped\n");
    } else if (strcmp("autoconfig", argv[1]) == 0) {
      ControllerAutoconfig();
    } else {
      dprint(ERR, "controller <start|stop|autoconfig>\n");
    }

  } while (0);
}

static void SnCmd(uint8_t argc, char *argv[]) {
  dprint(OK, "");

  // Print 96-bit serial number
  for (uint8_t byte = 0; byte < 12; byte++) {
    dprint(OK_CONT, "%02X ", uid[byte]);
  }
  dprint(OK_CONT, "\n");
}

static void VersionCmd(uint8_t argc, char *argv[]) {
  dprint(OK, "%s\n", versionStr);
}

static void ResetCmd(uint8_t argc, char *argv[]) { NVIC_SystemReset(); }

void ConsoleProcess() {
  uint32_t inBytes = FifoSize(&rxFifo);
  if (inBytes > 0) {
    uint32_t newLine = 0;
    for (int32_t index = 0; index < inBytes; index++) {
      if ((FifoPeek(&rxFifo, index) == '\n') ||
          (FifoPeek(&rxFifo, index) == '\r')) {
        newLine = index + 1;
        break;
      }
    }

    if (newLine > sizeof(cmdBuff)) {
      newLine = sizeof(cmdBuff) - 1;
    }

    if (newLine) {
      uint8_t *pBuf = (uint8_t *)cmdBuff;
      while (newLine--) {
        *pBuf++ = FifoPop(&rxFifo);
      }

      // If it's an \r\n combination, discard the second one
      if ((FifoPeek(&rxFifo, 0) == '\n') || (FifoPeek(&rxFifo, 0) == '\r')) {
        FifoPop(&rxFifo);
      }

      *(pBuf - 1) = 0;  // String terminator

      argc = 0;

      // Get command
      argv[argc] = strtok(cmdBuff, " ");

      // Get arguments (if any)
      while ((argc < sizeof(argv) / sizeof(char *)) && (argv[argc] != NULL)) {
        argc++;
        argv[argc] = strtok(NULL, " ");
      }

      if (argc > 0) {
        command_t *command = commands;
        while (command->commandStr != NULL) {
          if (strcmp(command->commandStr, argv[0]) == 0) {
            command->fn(argc, argv);
            break;
          }
          command++;
        }

        if (command->commandStr == NULL) {
          dprint(ERR, "Unknown command '%s'\n", argv[0]);
        }
      }
    }
  }
}
