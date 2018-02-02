#include "rtc.h"
#include <stdio.h>
#include "board.h"
#include "debug.h"
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "timer.h"

static char time_str[16];

int32_t RtcInit() {
  RTC_InitTypeDef RtcInit_struct;
  uint32_t blinkState = 0;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  PWR_BackupAccessCmd(ENABLE);

  dprint(INFO, "Enabling 32.768kHz oscillator.\n");
  RCC_LSEDriveConfig(RCC_LSEDrive_Low);
  RCC_LSEConfig(RCC_LSE_ON);

  while (!(RCC->BDCR & 0x2)) {
    if (blinkState) {
      GPIO_SetBits(LED0_PORT, (1 << LED0_PIN));
    } else {
      GPIO_ResetBits(LED0_PORT, (1 << LED0_PIN));
    }
    blinkState ^= 1;
    SleepMs(50);
  }

  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  RCC_RTCCLKCmd(ENABLE);

  RTC_WriteProtectionCmd(DISABLE);
  RTC_StructInit(&RtcInit_struct);
  RTC_Init(&RtcInit_struct);
  RTC_WriteProtectionCmd(ENABLE);

  return 0;
}

int32_t RtcSetTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                     uint8_t minute, uint8_t second) {
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;
  RTC_TimeStructInit(&time);
  RTC_DateStructInit(&date);

  date.RTC_Year = (year - 2000);
  date.RTC_Month = month;
  date.RTC_Date = day;

  time.RTC_Hours = hour;
  time.RTC_Minutes = minute;
  time.RTC_Seconds = second;

  RTC_SetTime(RTC_Format_BIN, &time);
  RTC_SetDate(RTC_Format_BIN, &date);

  // This seems to immediately update the date, instead of just after reset
  RTC_BypassShadowCmd(ENABLE);
  return 0;
}

char *RtcGetTimeStr() {
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;

  RTC_GetDate(RTC_Format_BCD, &date);
  RTC_GetTime(RTC_Format_BCD, &time);

  snprintf(time_str, sizeof(time_str), "20%02X%02X%02XT%02X%02X%02X",
           date.RTC_Year, date.RTC_Month, date.RTC_Date, time.RTC_Hours,
           time.RTC_Minutes, time.RTC_Seconds);

  return time_str;
}
