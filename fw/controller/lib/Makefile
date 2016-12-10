# Build all libraries

MAKEDEFS = FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR)

all:
	$(MAKE) -C STM32F0xx_StdPeriph_Driver $(MAKEDEFS)

clean:
	$(MAKE) clean -C STM32F0xx_StdPeriph_Driver
