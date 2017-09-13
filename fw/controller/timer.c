#include "timer.h"
#include "stm32f0xx.h"

volatile uint32_t tickMs = 0;

void sleep_ms(uint32_t milliseconds) {
  uint32_t start_time = get_tick_ms();
  while ((get_tick_ms() - start_time) < milliseconds) {
    __WFI();
  }
}

uint32_t get_tick_ms() { return tickMs; }

void timer_set(ms_timer_t *timer, uint32_t delay) {
  timer->start_time = get_tick_ms();
  timer->delay = delay;
}

void timer_clear(ms_timer_t *timer) { timer->delay = 0; }

bool timer_expired(ms_timer_t *timer) {
  if (timer->delay != 0) {
    if ((get_tick_ms() - timer->start_time) >= timer->delay) {
      return true;
    }
  }

  return false;
}

void SysTick_Handler(void) { tickMs++; }
