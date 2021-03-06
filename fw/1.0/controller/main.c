#include <stdint.h>
#include <stdio.h>
#include "board.h"
#include "config.h"
#include "console.h"
#include "controller.h"
#include "debug.h"
#include "i2c.h"
#include "rtc.h"
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "timer.h"
#include "uart.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"

#define BLINK_DELAY_MS (500)

USB_CORE_HANDLE USB_Device_dev;

void init() {
  SystemCoreClockUpdate();

  // ---------- SysTick timer -------- //
  if (SysTick_Config(SystemCoreClock / 1000)) {
    // Capture error
    while (1) {
    };
  }

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);

  GPIO_Init(LED0_PORT,
            &(GPIO_InitTypeDef){(1 << LED0_PIN), GPIO_Mode_OUT, GPIO_Speed_2MHz,
                                GPIO_OType_PP, GPIO_PuPd_NOPULL});

  GPIO_Init(LED1_PORT,
            &(GPIO_InitTypeDef){(1 << LED1_PIN), GPIO_Mode_OUT, GPIO_Speed_2MHz,
                                GPIO_OType_PP, GPIO_PuPd_NOPULL});

  GPIO_Init(LED2_PORT,
            &(GPIO_InitTypeDef){(1 << LED2_PIN), GPIO_Mode_OUT, GPIO_Speed_2MHz,
                                GPIO_OType_PP, GPIO_PuPd_NOPULL});

  GPIO_Init(
      TCA_nRST_PORT,
      &(GPIO_InitTypeDef){(1 << TCA_nRST_PIN), GPIO_Mode_OUT, GPIO_Speed_2MHz,
                          GPIO_OType_PP, GPIO_PuPd_NOPULL});

  GPIO_Init(FAN_PORT,
            &(GPIO_InitTypeDef){(1 << FAN_PIN), GPIO_Mode_OUT, GPIO_Speed_2MHz,
                                GPIO_OType_PP, GPIO_PuPd_NOPULL});
  GPIO_ResetBits(FAN_PORT, (1 << FAN_PIN));

  GPIO_Init(FRIDGE_PORT, &(GPIO_InitTypeDef){(1 << FRIDGE_PIN), GPIO_Mode_OUT,
                                             GPIO_Speed_2MHz, GPIO_OType_PP,
                                             GPIO_PuPd_NOPULL});
  GPIO_ResetBits(FRIDGE_PORT, (1 << FRIDGE_PIN));

  GPIO_Init(PUMP_PORT,
            &(GPIO_InitTypeDef){(1 << PUMP_PIN), GPIO_Mode_OUT, GPIO_Speed_2MHz,
                                GPIO_OType_PP, GPIO_PuPd_NOPULL});
  GPIO_ResetBits(PUMP_PORT, (1 << PUMP_PIN));

  GPIO_Init(MIST_PORT,
            &(GPIO_InitTypeDef){(1 << MIST_PIN), GPIO_Mode_OUT, GPIO_Speed_2MHz,
                                GPIO_OType_PP, GPIO_PuPd_NOPULL});
  GPIO_ResetBits(MIST_PORT, (1 << MIST_PIN));

  GPIO_SetBits(TCA_nRST_PORT, (1 << TCA_nRST_PIN));

  UartInit(115200);

  fprintf(stderr, "Ostur Controller %s\n", FW_VERSION);

  USBD_Init(&USB_Device_dev, &USR_desc, &USBD_CDC_cb, &USR_cb);

  // If we try to output anything over USB before it's ready, it stops
  // working completely. Here we wait until it's ready (or times out)
  ms_timer_t usb_config_timer;
  TimerSet(&usb_config_timer, 1000);
  while ((USB_Device_dev.dev.device_status != USB_CONFIGURED) &&
         !TimerExpired(&usb_config_timer)) {
    SleepMs(2);  // No need to spin constantly
  }

  if (USB_Device_dev.dev.device_status != USB_CONFIGURED) {
    fprintf(stderr, "USB never came up.\n");
  }

  RtcInit();

  i2cSetup(100000);
  ConfigInit();
  ControllerInit();
}

int main(void) {
  ms_timer_t blink_timer;
  uint32_t blinkState = 0;

  init();

  TimerSet(&blink_timer, BLINK_DELAY_MS);
  for (;;) {
    if (TimerExpired(&blink_timer)) {
      TimerSet(&blink_timer, BLINK_DELAY_MS);
      if (blinkState) {
        GPIO_SetBits(LED0_PORT, (1 << LED0_PIN));
      } else {
        GPIO_ResetBits(LED0_PORT, (1 << LED0_PIN));
      }
      blinkState ^= 1;
    }

    ControllerProcess();
    ConsoleProcess();

    __WFI();
  }

  return 0;
}
