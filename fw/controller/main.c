#include <stdio.h>
#include <stdint.h>

#include "uart.h"
#include "console.h"
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"

#define BLINK_DELAY_MS	(500)


volatile uint32_t tickMs = 0;

void init() {
	// ---------- SysTick timer -------- //
	if (SysTick_Config(SystemCoreClock / 1000)) {
		// Capture error
		while (1){};
	}

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_Init(GPIOB, &(GPIO_InitTypeDef){GPIO_Pin_0, GPIO_Mode_OUT, GPIO_Speed_2MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL});
	GPIO_Init(GPIOB, &(GPIO_InitTypeDef){GPIO_Pin_1, GPIO_Mode_OUT, GPIO_Speed_2MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL});
	GPIO_Init(GPIOB, &(GPIO_InitTypeDef){GPIO_Pin_2, GPIO_Mode_OUT, GPIO_Speed_2MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL});

	uartInit(115200);
}

int main(void) {
	uint32_t nextBlink;
	uint32_t blinkState = 0;

	init();

	nextBlink = tickMs + BLINK_DELAY_MS;
	for(;;) {

		if(tickMs > nextBlink) {
			nextBlink = tickMs + BLINK_DELAY_MS;
			if(blinkState) {
				GPIO_SetBits(GPIOB, GPIO_Pin_0);
			} else {
				GPIO_ResetBits(GPIOB, GPIO_Pin_0);
			}
			blinkState ^= 1;
		}

		consoleProcess();

		__WFI();
	}

	return 0;
}

void SysTick_Handler(void)
{
	tickMs++;
}



