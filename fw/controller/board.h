#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm32f0xx.h"

#define LED0_PORT GPIOB
#define LED0_PIN 0

#define LED1_PORT GPIOB
#define LED1_PIN 1

#define LED2_PORT GPIOB
#define LED2_PIN 2

#define UART1_TX_PORT GPIOA
#define UART1_TX_PIN 9

#define UART1_RX_PORT GPIOA
#define UART1_RX_PIN 10

#define SDA_PORT GPIOB
#define SDA_PIN 7

#define SCL_PORT GPIOB
#define SCL_PIN 6

#define TCA_nRST_PORT GPIOB
#define TCA_nRST_PIN 8

#define MIST_PORT GPIOA
#define MIST_PIN 3

#define PUMP_PORT GPIOA
#define PUMP_PIN 4

#define FAN_PORT GPIOA
#define FAN_PIN 5

#define FRIDGE_PORT GPIOA
#define FRIDGE_PIN 6

#endif  // __BOARD__
