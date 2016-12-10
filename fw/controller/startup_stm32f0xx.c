

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
/* System exception vector handler */
void __attribute__ ((weak)) Reset_Handler(void);
void __attribute__ ((weak)) NMI_Handler(void);
void __attribute__ ((weak)) HardFault_Handler(void);
void __attribute__ ((weak)) SVC_Handler(void);
void __attribute__ ((weak)) PendSV_Handler(void);
void __attribute__ ((weak)) SysTick_Handler(void);

/* External interrupt vector handler */
void __attribute__ ((weak)) WWDG_IRQHandler(void);
void __attribute__ ((weak)) PVD_VDDIO2_IRQHandler(void);
void __attribute__ ((weak)) RTC_IRQHandler(void);
void __attribute__ ((weak)) FLASH_IRQHandler(void);
void __attribute__ ((weak)) RCC_CRS_IRQHandler(void);
void __attribute__ ((weak)) EXTI0_1_IRQHandler(void);
void __attribute__ ((weak)) EXTI2_3_IRQHandler(void);
void __attribute__ ((weak)) EXTI4_15_IRQHandler(void);
void __attribute__ ((weak)) TSC_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Channel1_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Channel2_3_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Channel4_5_6_7_IRQHandler(void);
void __attribute__ ((weak)) ADC1_COMP_IRQHandler(void);
void __attribute__ ((weak)) TIM1_BRK_UP_TRG_COM_IRQHandler(void);
void __attribute__ ((weak)) TIM1_CC_IRQHandler(void);
void __attribute__ ((weak)) TIM2_IRQHandler(void);
void __attribute__ ((weak)) TIM3_IRQHandler(void);
void __attribute__ ((weak)) TIM6_DAC_IRQHandler(void);
void __attribute__ ((weak)) TIM7_IRQHandler(void);
void __attribute__ ((weak)) TIM14_IRQHandler(void);
void __attribute__ ((weak)) TIM15_IRQHandler(void);
void __attribute__ ((weak)) TIM16_IRQHandler(void);
void __attribute__ ((weak)) TIM17_IRQHandler(void);
void __attribute__ ((weak)) I2C1_IRQHandler(void);
void __attribute__ ((weak)) I2C2_IRQHandler(void);
void __attribute__ ((weak)) SPI1_IRQHandler(void);
void __attribute__ ((weak)) SPI2_IRQHandler(void);
void __attribute__ ((weak)) USART1_IRQHandler(void);
void __attribute__ ((weak)) USART2_IRQHandler(void);
void __attribute__ ((weak)) USART3_4_IRQHandler (void);
void __attribute__ ((weak)) CEC_CAN_IRQHandler(void);
void __attribute__ ((weak)) USB_IRQHandler(void);
void __attribute__ ((weak)) BootRAM(void);

/* Exported types --------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern unsigned long __etext;
extern unsigned long _sidata;			/* start address for the initialization values of the .data section. defined in linker script */
extern unsigned long _sdata;	/* start address for the .data section. defined in linker script */
extern unsigned long _edata;		/* end address for the .data section. defined in linker script */

extern unsigned long __bss_start__;		/* start address for the .bss section. defined in linker script */
extern unsigned long __bss_end__;		/* end address for the .bss section. defined in linker script */

extern unsigned long *_estack;				/* init value for the stack pointer. defined in linker script */

extern void SystemInit(void);

/* Private typedef -----------------------------------------------------------*/
/* function prototypes ------------------------------------------------------*/
void Reset_Handler(void) __attribute__((__interrupt__));
extern int main(void);

__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{	
	(void *)&_estack,
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	SVC_Handler,
	0,
	0,
	PendSV_Handler,
	SysTick_Handler,
	WWDG_IRQHandler,
	PVD_VDDIO2_IRQHandler,
	RTC_IRQHandler,
	FLASH_IRQHandler,
	RCC_CRS_IRQHandler,
	EXTI0_1_IRQHandler,
	EXTI2_3_IRQHandler,
	EXTI4_15_IRQHandler,
	TSC_IRQHandler,
	DMA1_Channel1_IRQHandler,
	DMA1_Channel2_3_IRQHandler,
	DMA1_Channel4_5_6_7_IRQHandler,
	ADC1_COMP_IRQHandler,
	TIM1_BRK_UP_TRG_COM_IRQHandler,
	TIM1_CC_IRQHandler,
	TIM2_IRQHandler,
	TIM3_IRQHandler,
	TIM6_DAC_IRQHandler,
	TIM7_IRQHandler,
	TIM14_IRQHandler,
	TIM15_IRQHandler,
	TIM16_IRQHandler,
	TIM17_IRQHandler,
	I2C1_IRQHandler,
	I2C2_IRQHandler,
	SPI1_IRQHandler,
	SPI2_IRQHandler,
	USART1_IRQHandler,
	USART2_IRQHandler,
	USART3_4_IRQHandler,
	CEC_CAN_IRQHandler,
	USB_IRQHandler,
	BootRAM,
};

/*******************************************************************************
* Function Name	: Reset_Handler
* Description	: This is the code that gets called when the processor first starts execution
*				following a reset event.	Only the absolutely necessary set is performed,
*				after which the application supplied main() routine is called.
* Input			:
* Output		:
* Return		:
*******************************************************************************/
void Reset_Handler(void) {
	unsigned long *pulSrc, *pulDest;

	/*
	* This used for cleaning AHBRAM0 section
	*/
#if 0
	for (pulDest = ((unsigned long *)AHBRAM0_BASE); \
					pulDest < ((unsigned long *)(AHBRAM0_BASE + AHBRAM0_SIZE)); \
					pulDest++){
		*(pulDest++) = 0;
	}
#endif

	/*
	* This used for cleaning AHBRAM1 section
	*/
#if 0
	for (pulDest = ((unsigned long *)AHBRAM1_BASE); \
					pulDest < ((unsigned long *)(AHBRAM1_BASE + AHBRAM1_SIZE)); \
					pulDest++){
		*(pulDest++) = 0;
	}
#endif

	//
	// Copy the data segment initializers from flash to SRAM in ROM mode
	//
#if (__RAM_MODE__==0)
	pulSrc = &_sidata;
	for(pulDest = &_sdata; pulDest < &_edata; )
	{
		*(pulDest++) = *(pulSrc++);
	}
#endif


	//
	// Zero fill the bss segment.
	//
	for(pulDest = &__bss_start__; pulDest < &__bss_end__; )
	{
		*(pulDest++) = 0;
	}

	SystemInit();

	//
	// Call the application's entry point.
	//
	main();
}

//*****************************************************************************
//
// Provide weak aliases for each Exception handler to the Default_Handler.
// As they are weak aliases, any function with the same name will override
// this definition.
//
//*****************************************************************************

#pragma weak SVC_Handler = Default_Handler
#pragma weak PendSV_Handler = Default_Handler
#pragma weak SysTick_Handler = Default_Handler
#pragma weak WWDG_IRQHandler = Default_Handler
#pragma weak PVD_VDDIO2_IRQHandler = Default_Handler
#pragma weak RTC_IRQHandler = Default_Handler
#pragma weak FLASH_IRQHandler = Default_Handler
#pragma weak RCC_CRS_IRQHandler = Default_Handler
#pragma weak EXTI0_1_IRQHandler = Default_Handler
#pragma weak EXTI2_3_IRQHandler = Default_Handler
#pragma weak EXTI4_15_IRQHandler = Default_Handler
#pragma weak TSC_IRQHandler = Default_Handler
#pragma weak DMA1_Channel1_IRQHandler = Default_Handler
#pragma weak DMA1_Channel2_3_IRQHandler = Default_Handler
#pragma weak DMA1_Channel4_5_6_7_IRQHandler = Default_Handler
#pragma weak ADC1_COMP_IRQHandler  = Default_Handler
#pragma weak TIM1_BRK_UP_TRG_COM_IRQHandler = Default_Handler
#pragma weak TIM1_CC_IRQHandler = Default_Handler
#pragma weak TIM2_IRQHandler = Default_Handler
#pragma weak TIM3_IRQHandler = Default_Handler
#pragma weak TIM6_DAC_IRQHandler = Default_Handler
#pragma weak TIM7_IRQHandler     = Default_Handler
#pragma weak TIM14_IRQHandler = Default_Handler
#pragma weak TIM15_IRQHandler = Default_Handler
#pragma weak TIM16_IRQHandler = Default_Handler
#pragma weak TIM17_IRQHandler = Default_Handler
#pragma weak I2C1_IRQHandler = Default_Handler
#pragma weak I2C2_IRQHandler = Default_Handler
#pragma weak SPI1_IRQHandler = Default_Handler
#pragma weak SPI2_IRQHandler = Default_Handler
#pragma weak USART1_IRQHandler = Default_Handler
#pragma weak USART2_IRQHandler = Default_Handler
#pragma weak USART3_4_IRQHandler  = Default_Handler
#pragma weak CEC_CAN_IRQHandler = Default_Handler
#pragma weak USB_IRQHandler = Default_Handler
#pragma weak BootRAM = Default_Handler

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.	This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
void Default_Handler(void) {
	while (1) {}
}
