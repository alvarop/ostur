#include "timer.h"
#include "stm32f0xx.h"

volatile uint32_t tickMs = 0;

void sleep_ms(uint32_t milliseconds) {
	uint32_t start_time = get_tick_ms();
	while((get_tick_ms() - start_time) < milliseconds)	{
		__WFI();
	}
}

uint32_t get_tick_ms() {
	return tickMs;
}

void SysTick_Handler(void)
{
	tickMs++;
}
