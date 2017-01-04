#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

void sleep_ms(uint32_t milliseconds);
uint32_t get_tick_ms();

#endif