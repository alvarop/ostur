#include "uart.h"
#include "board.h"
#include "fifo.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"

#define FIFO_BUFF_SIZE  (2048)

extern uint16_t VCP_DataTx (uint8_t* Buf, uint32_t Len);

fifo_t txFifo;
fifo_t rxFifo;
static uint8_t outBuff[FIFO_BUFF_SIZE];
static uint8_t inBuff[FIFO_BUFF_SIZE];

void uartInit(uint32_t baud) {
	USART_InitTypeDef uartConfig;

	fifoInit(&txFifo, FIFO_BUFF_SIZE, outBuff);
	fifoInit(&rxFifo, FIFO_BUFF_SIZE, inBuff);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_Init(UART1_TX_PORT,
		&(GPIO_InitTypeDef){
			(1 << UART1_TX_PIN),
			GPIO_Mode_AF,
			GPIO_Speed_50MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL});

	GPIO_Init(UART1_RX_PORT,
		&(GPIO_InitTypeDef){
			(1 << UART1_RX_PIN),
			GPIO_Mode_AF,
			GPIO_Speed_50MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL});

	GPIO_PinAFConfig(UART1_TX_PORT, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(UART1_RX_PORT, GPIO_PinSource10, GPIO_AF_1);

	USART_StructInit(&uartConfig);

	uartConfig.USART_BaudRate = baud;
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
	// Stderr only goes to uart, not USB CDC
	// Stderr is #2
	if(fd != 2) {
		VCP_DataTx((uint8_t *)ptr, len);
	}

	// Print everything to stderr for now
	for(uint32_t byte =0; byte < len; byte++) {
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
    readChars++;
  }

  return readChars;
}

void USART1_IRQHandler(void) {
	uint32_t irq = USART1->ISR;
	if(irq & USART_ISR_TXE) {
		// Tx new byte if available
		if(fifoSize(&txFifo) > 0) {
			USART1->TDR = fifoPop(&txFifo);

		} else {
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

		}
	}

	if(irq & USART_ISR_RXNE) {
		// Add received char to buff
		fifoPush(&rxFifo, USART1->RDR);
	}

	USART1->ICR = irq;
}
