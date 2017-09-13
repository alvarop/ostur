#ifndef __RTC_H__
#define __RTC_H__

#include <stdint.h>

int32_t RtcInit();
int32_t RtcSetTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                     uint8_t minute, uint8_t second);
char *RtcGetTimeStr();

#endif
