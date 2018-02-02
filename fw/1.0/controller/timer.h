#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint32_t start_time;
  uint32_t delay;
} ms_timer_t;

void SleepMs(uint32_t milliseconds);
uint32_t GetTickMs();

void TimerSet(ms_timer_t *timer, uint32_t delay);
void TimerClear(ms_timer_t *timer);
bool TimerExpired(ms_timer_t *timer);

#endif