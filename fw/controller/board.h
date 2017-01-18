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

#define TCA_A2_PORT GPIOB
#define TCA_A2_PIN 9

#define MIST_PORT GPIOB
#define MIST_PIN 15

#define PUMP_PORT GPIOB
#define PUMP_PIN 14

#define FAN_PORT GPIOB
#define FAN_PIN 13

#define FRIDGE_PORT GPIOB
#define FRIDGE_PIN 12

#endif // __BOARD__
