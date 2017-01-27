#ifndef __RTC_H__
#define __RTC_H__

#include <stdint.h>

int32_t rtc_init();
int32_t rtc_set_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
char *rtc_get_time_str();

#endif
