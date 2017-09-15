#include "timer.h"
#include "stm32f0xx.h"

volatile uint32_t tickMs = 0;

void SleepMs(uint32_t milliseconds) {
  uint32_t start_time = GetTickMs();
  while ((GetTickMs() - start_time) < milliseconds) {
    __WFI();
  }
}

uint32_t GetTickMs() { return tickMs; }

void TimerSet(ms_timer_t *timer, uint32_t delay) {
  timer->start_time = GetTickMs();
  timer->delay = delay;
}

void TimerClear(ms_timer_t *timer) { timer->delay = 0; }

bool TimerExpired(ms_timer_t *timer) {
  if (timer->delay != 0) {
    if ((GetTickMs() - timer->start_time) >= timer->delay) {
      return true;
    }
  }

  return false;
}

void SysTick_Handler(void) { tickMs++; }
