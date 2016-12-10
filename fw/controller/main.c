#include <stdio.h>
#include <stdint.h>

#include "console.h"
#include "fifo.h"
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"

#include "stm32f0xx_usart.h"

#define BLINK_DELAY_MS	(500)
#define FIFO_BUFF_SIZE  (512)

fifo_t txFifo;
fifo_t rxFifo;
static uint8_t outBuff[FIFO_BUFF_SIZE];
static uint8_t inBuff[FIFO_BUFF_SIZE];

volatile uint32_t tickMs = 0;

void init_uart() {
	USART_InitTypeDef uartConfig;

	fifoInit(&txFifo, FIFO_BUFF_SIZE, outBuff);
	fifoInit(&rxFifo, FIFO_BUFF_SIZE, inBuff);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &(GPIO_InitTypeDef){GPIO_Pin_9, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL});
	GPIO_Init(GPIOA, &(GPIO_InitTypeDef){GPIO_Pin_10, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL});
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

	USART_StructInit(&uartConfig);

	uartConfig.USART_BaudRate = 115200;
	USART_Init(USART1, &uartConfig);


	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

	USART_Cmd(USART1, ENABLE);

	NVIC_EnableIRQ(USART1_IRQn);
}

int uartPutchar(USART_TypeDef *uart, fifo_t *fifo, char c) {
	if(uart->ISR & USART_ISR_TXE) {
		uart->TDR = c;
		USART_ITConfig(uart, USART_IT_TXE, ENABLE);
		GPIO_SetBits(GPIOB, GPIO_Pin_1);

	} else {
		fifoPush(fifo, c);
	}

	return c;
}

//
// Retarget read/write to use usb/serial!
// Found in share/gcc-arm-none-eabi/samples/src/retarget/retarget.c
//
int _write (int fd, char *ptr, int len)
{
	//
	// If planning on supporting both serial and usb-serial, check fd here!
	//
	while(len--) {
		uartPutchar(USART1, &txFifo, *ptr++);
	}
	return len;
}

int _read (int fd, char *ptr, int len)
{
  int readChars = 0;
  
  //
  // If planning on supporting both serial and usb-serial, check fd here!
  //
  while(fifoSize(&rxFifo) && len--) {
    *ptr++ = fifoPop(&rxFifo);
  }
  
  return readChars;
}

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

	init_uart();
}

int main(void) {
	uint32_t nextBlink;
	uint32_t blinkState = 0;

	init();

	printf("hello!\n");

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

void USART1_IRQHandler(void) {
	uint32_t irq = USART1->ISR;
	if(irq & USART_ISR_TXE) {
		// Tx new byte if available
		if(fifoSize(&txFifo) > 0) {
			USART1->TDR = fifoPop(&txFifo);
			
		} else {
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			GPIO_ResetBits(GPIOB, GPIO_Pin_1);

		}
	}

	if(irq & USART_ISR_RXNE) {
		// Add received char to buff
		fifoPush(&rxFifo, USART1->RDR);
		GPIO_SetBits(GPIOB, GPIO_Pin_2);
	}

	USART1->ICR = irq;
}

