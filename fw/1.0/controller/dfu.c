#include "stm32f0xx.h"

#define BOTTOM_OF_RAM 0x20000000
#define DFU_ENABLE_CODE 0xB00710AD

#define DFU_STACK_POINTER 0x20002250
#define DFU_START_ADDR 0x1FFFC800

// Does not return, sets magic code and restarts device
void EnterDfu() {
  uint32_t *dfu_enable_flag = (uint32_t *)(BOTTOM_OF_RAM);

  // Disable interrupts
  __disable_irq();

  // Set magic code
  *dfu_enable_flag = DFU_ENABLE_CODE;
  NVIC_SystemReset();
}

static void (*DfuResetVector)(void);

// Call first thing in Reset_Handler so we can enter booloader if needed
void DfuCheck() {
  uint32_t *dfu_enable_flag = (uint32_t *)(BOTTOM_OF_RAM);

  // If magic code is set, DFU this thing!
  if (*dfu_enable_flag == DFU_ENABLE_CODE) {
    *dfu_enable_flag = 0;

    // Set the stack pointer to the bootloader's
    __set_MSP(*(uint32_t *)(DFU_START_ADDR));

    // Reset handler is 4 bytes from the start of DFU memory
    DfuResetVector = (void (*)(void))(*((uint32_t *)(DFU_START_ADDR + 4)));

    // Enter the bootloader!
    DfuResetVector();

    while (1)
      ;
  }
}
