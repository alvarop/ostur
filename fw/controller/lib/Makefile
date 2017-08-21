# Build all libraries

MAKEDEFS = FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR)

all:
	$(MAKE) -C STM32F0xx_StdPeriph_Driver $(MAKEDEFS)
	$(MAKE) -C STM32_USB_Device_Library/Core $(MAKEDEFS)
	$(MAKE) -C STM32_USB_Device_Driver $(MAKEDEFS)

clean:
	$(MAKE) clean -C STM32F0xx_StdPeriph_Driver
	$(MAKE) clean -C STM32_USB_Device_Library/Core
	$(MAKE) clean -C STM32_USB_Device_Driver
