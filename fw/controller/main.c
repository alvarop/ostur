#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "i2c.h"
#include "uart.h"
#include "console.h"
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "timer.h"
#include "config.h"
#include "controller.h"

#define BLINK_DELAY_MS	(500)

void init() {
	// ---------- SysTick timer -------- //
	if (SysTick_Config(SystemCoreClock / 1000)) {
		// Capture error
		while (1){};
	}

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_Init(LED0_PORT,
		&(GPIO_InitTypeDef){
			(1 << LED0_PIN),
			GPIO_Mode_OUT,
			GPIO_Speed_2MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL});

	GPIO_Init(LED1_PORT,
		&(GPIO_InitTypeDef){
			(1 << LED1_PIN),
			GPIO_Mode_OUT,
			GPIO_Speed_2MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL});

	GPIO_Init(LED2_PORT,
		&(GPIO_InitTypeDef){
			(1 << LED2_PIN),
			GPIO_Mode_OUT,
			GPIO_Speed_2MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL});

	GPIO_Init(TCA_nRST_PORT,
		&(GPIO_InitTypeDef){
			(1 << TCA_nRST_PIN),
			GPIO_Mode_OUT,
			GPIO_Speed_2MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL});

	GPIO_Init(TCA_A2_PORT,
		&(GPIO_InitTypeDef){
			(1 << TCA_A2_PIN),
			GPIO_Mode_OUT,
			GPIO_Speed_2MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL});

	GPIO_Init(FAN_PORT,
		&(GPIO_InitTypeDef){
			(1 << FAN_PIN),
			GPIO_Mode_OUT,
			GPIO_Speed_2MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL});
	GPIO_ResetBits(FAN_PORT, (1 << FAN_PIN));

	GPIO_Init(FRIDGE_PORT,
		&(GPIO_InitTypeDef){
			(1 << FRIDGE_PIN),
			GPIO_Mode_OUT,
			GPIO_Speed_2MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL});
	GPIO_ResetBits(FRIDGE_PORT, (1 << FRIDGE_PIN));

	GPIO_Init(PUMP_PORT,
		&(GPIO_InitTypeDef){
			(1 << PUMP_PIN),
			GPIO_Mode_OUT,
			GPIO_Speed_2MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL});
	GPIO_ResetBits(PUMP_PORT, (1 << PUMP_PIN));

	GPIO_Init(MIST_PORT,
		&(GPIO_InitTypeDef){
			(1 << MIST_PIN),
			GPIO_Mode_OUT,
			GPIO_Speed_2MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL});
	GPIO_ResetBits(MIST_PORT, (1 << MIST_PIN));

	GPIO_SetBits(TCA_nRST_PORT, (1 << TCA_nRST_PIN));
	GPIO_ResetBits(TCA_A2_PORT, (1 << TCA_A2_PIN));

	uartInit(115200);
	i2cSetup(100000);
	config_init();
	controller_init();
}

int main(void) {
	ms_timer_t blink_timer;
	uint32_t blinkState = 0;

	init();

	timer_set(&blink_timer, BLINK_DELAY_MS);
	for(;;) {

		if(timer_expired(&blink_timer)) {
			timer_set(&blink_timer, BLINK_DELAY_MS);
			if(blinkState) {
				GPIO_SetBits(LED0_PORT, (1 << LED0_PIN));
			} else {
				GPIO_ResetBits(LED0_PORT, (1 << LED0_PIN));
			}
			blinkState ^= 1;
		}

		controller_process();
		consoleProcess();

		__WFI();
	}

	return 0;
}
