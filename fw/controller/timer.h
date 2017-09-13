#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint32_t start_time;
  uint32_t delay;
} ms_timer_t;

void sleep_ms(uint32_t milliseconds);
uint32_t get_tick_ms();

void timer_set(ms_timer_t *timer, uint32_t delay);
void timer_clear(ms_timer_t *timer);
bool timer_expired(ms_timer_t *timer);

#endif