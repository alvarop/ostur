#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx.h"
#include "i2c.h"

#define I2C_TIMEOUT_MS (50)

extern volatile uint32_t tickMs;
static volatile uint32_t i2cErr = 0;

void I2C1_IRQHandler(void) {
	uint32_t isr = I2C1->ISR;
	I2C1->ICR = isr;
	i2cErr = isr;
	I2C_ITConfig(I2C1, I2C_IT_ERRI, DISABLE);
}

void i2cSetup(uint32_t speed) {
	I2C_InitTypeDef i2cConfig;

	// GPIO Init
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_PinAFConfig(SDA_PORT, SDA_PIN, GPIO_AF_1);
	GPIO_PinAFConfig(SCL_PORT, SCL_PIN, GPIO_AF_1);

	GPIO_Init(SDA_PORT,
		&(GPIO_InitTypeDef){
			(1 << SDA_PIN),
			GPIO_Mode_AF,
			GPIO_Speed_50MHz,
			GPIO_OType_OD,
			GPIO_PuPd_NOPULL});

	GPIO_Init(SCL_PORT,
		&(GPIO_InitTypeDef){
			(1 << SCL_PIN),
			GPIO_Mode_AF,
			GPIO_Speed_50MHz,
			GPIO_OType_OD,
			GPIO_PuPd_NOPULL});

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	// I2C init
	I2C_StructInit(&i2cConfig);

	i2cConfig.I2C_Timing = 0x2000090E;

	I2C_DeInit(I2C1);

	I2C_Init(I2C1, &i2cConfig);

	// I2C_ITConfig(I2C1, I2C_IT_ERRI, ENABLE);
	// NVIC_EnableIRQ(I2C1_IRQn);

	I2C_Cmd(I2C1, ENABLE);
}

int32_t i2c(I2C_TypeDef* I2Cx, uint8_t addr, uint16_t wLen, uint8_t *wBuff, uint16_t rLen, uint8_t *rBuff) {
	int32_t rval = 0;
	uint32_t reg;
	uint32_t timeout;

	do {
		i2cErr = 0;

		timeout = tickMs + I2C_TIMEOUT_MS;
		while((I2Cx->ISR & I2C_ISR_BUSY) && (tickMs < timeout)) {
		}

		if (tickMs >= timeout) {
			rval = I2C_TIMEOUT;
			break;
		}

		I2C_ITConfig(I2C1, I2C_IT_ERRI, ENABLE);

		if(wLen > 0) {
			I2Cx->CR2 = (addr & 0xFE); // Add slave address
			I2Cx->CR2 |= ((wLen & 0xFF) << 16); // Add nbytes
			I2Cx->CR2 |= I2C_CR2_START;

			// Wait for address to be sent
			do {
				reg = I2Cx->CR2;
			} while((reg & I2C_CR2_START) && !i2cErr && (tickMs < timeout));

			if (tickMs >= timeout) {
				rval = I2C_ERR;
				break;
			}

			if(i2cErr & I2C_ISR_NACKF) {
				I2Cx->ICR = I2C_ICR_NACKCF; // Clear ack failure bit
				rval = I2C_ANACK;
				break;
			}

			while(wLen--) {
				do {
					reg = I2Cx->ISR;
				} while(!(reg & (I2C_ISR_TXIS | I2C_ISR_NACKF)) && !i2cErr && (tickMs < timeout));

				I2Cx->TXDR = *wBuff++;

				if (tickMs >= timeout) {
					rval = I2C_ERR;
					break;
				}

				if(i2cErr & I2C_ISR_NACKF) {
					I2Cx->ICR = I2C_ICR_NACKCF; // Clear ack failure bit
					rval = I2C_DNACK;
					break;
				}
			}

			// Some error occurred
			if(rval) {
				break;
			}

			// Wait for transfer complete bit
			do {
				reg = I2Cx->ISR;
			} while(!(reg & I2C_ISR_TC) && !i2cErr && (tickMs < timeout));

			if (tickMs >= timeout) {
				rval = I2C_TIMEOUT;
				break;
			}
		}

		if(rLen > 0) {
			I2Cx->CR2 = (addr & 0xFE); // Add slave address
			I2Cx->CR2 |= ((rLen & 0xFF) << 16); // Add nbytes
			I2Cx->CR2 |= I2C_CR2_RD_WRN;
			I2Cx->CR2 |= I2C_CR2_START;

			// Wait for address to be sent
			do {
				reg = I2Cx->CR2;
			} while((reg & I2C_CR2_START) && !i2cErr && (tickMs < timeout));

			if (tickMs >= timeout) {
				rval = I2C_ERR;
				break;
			}

			if(i2cErr & I2C_ISR_NACKF) {
				I2Cx->ICR = I2C_ICR_NACKCF; // Clear ack failure bit
				rval = I2C_ANACK;
				break;
			}

			while(rLen--) {
				do {
					reg = I2Cx->ISR;
				} while(!(reg & (I2C_ISR_RXNE)) && !i2cErr && (tickMs < timeout));

				if (tickMs >= timeout) {
					rval = I2C_ERR;
					break;
				}

				*rBuff++ = I2Cx->RXDR;
			}

			// Wait for transfer complete bit
			do {
				reg = I2Cx->ISR;
			} while(!(reg & I2C_ISR_TC) && !i2cErr && (tickMs < timeout));

			if (tickMs >= timeout) {
				rval = I2C_TIMEOUT;
			}
		}

	} while(0);

	// Generate stop condition
	I2Cx->CR2 |= I2C_CR2_STOP;

	I2C_ITConfig(I2C1, I2C_IT_ERRI, DISABLE);

	// Flush the transmit data register if needed (in case of NACK, etc.)
	if((I2C1->ISR & I2C_ISR_TXE) == 0) {
		I2C1->ISR |= I2C_ISR_TXE;
	}

	return rval;
}
