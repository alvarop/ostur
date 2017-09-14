# Sources

SRCS =	main.c \
		fifo.c \
		i2c.c \
		uart.c \
		console.c \
		startup_stm32f0xx.c \
		tca95xxa.c \
		timer.c \
		sht31.c \
		config.c \
		controller.c \
		debug.c \
		rtc.c

SRCS += system_stm32f0xx.c\
		stm32f0xx_it.c

# USB sources
USB_SRCS += usb/usbd_desc.c \
			usb/usbd_cdc_core.c \
			usb/usbd_cdc_vcp.c \
			usb/usb_bsp.c \
			usb/usbd_usr.c \
			usb/usbd_pwr.c

S_SRCS =

# Project name

PROJ_NAME = controller
OUTPATH = build

OUTPATH := $(abspath $(OUTPATH))

BASEDIR := $(abspath ./)

AS=$(BINPATH)arm-none-eabi-as
CC=$(BINPATH)arm-none-eabi-gcc
LD=$(BINPATH)arm-none-eabi-gcc
OBJCOPY=$(BINPATH)arm-none-eabi-objcopy
OBJDUMP=$(BINPATH)arm-none-eabi-objdump
SIZE=$(BINPATH)arm-none-eabi-size

LINKER_SCRIPT = stm32f072vb_flash.ld

CPU = -mcpu=cortex-m0 -mthumb

VERSION = $(shell git describe --dirty | sed 's/releases\/fw\/controller\///')

CFLAGS  = $(CPU) -c -std=gnu99 -g -O2 -Wall -Werror
LDFLAGS  = $(CPU) -mlittle-endian -mthumb-interwork -nostartfiles -Wl,--gc-sections,-Map=$(OUTPATH)/$(PROJ_NAME).map,--cref --specs=nano.specs

CFLAGS += -msoft-float

# Default to STM32F072 if no device is passed
ifeq ($(DEVICE_DEF), )
DEVICE_DEF = STM32F072
endif

CFLAGS += -D$(DEVICE_DEF)
CFLAGS += -DFW_VERSION=\"$(VERSION)\"

#vpath %.c src
vpath %.a lib

# Includes
INCLUDE_PATHS = -I$(BASEDIR) -I$(BASEDIR)/lib/CMSIS/Include -I$(BASEDIR)/lib/CMSIS/Device/ST/STM32F0xx/Include
INCLUDE_PATHS += -I$(BASEDIR)/lib/STM32_USB_Device_Driver/inc
INCLUDE_PATHS += -I$(BASEDIR)/lib/STM32_USB_Device_Library/Core/inc

# Library paths
LIBPATHS = -L$(BASEDIR)/lib/STM32F0xx_StdPeriph_Driver/
LIBPATHS += -L$(BASEDIR)/lib/STM32_USB_Device_Driver/
LIBPATHS += -L$(BASEDIR)/lib/STM32_USB_Device_Library/Core/

# Libraries to link
LIBS = -lstdperiph -lc -lgcc -lnosys
LIBS += -lusbdevcore -lusbcore

# Extra includes
INCLUDE_PATHS += -I$(BASEDIR)/lib/STM32F0xx_StdPeriph_Driver/inc
INCLUDE_PATHS += -I$(BASEDIR)/lib/STM32_USB_Device_Driver/inc
INCLUDE_PATHS += -I$(BASEDIR)/lib/STM32_USB_Device_Library/Core/inc
INCLUDE_PATHS += -I$(BASEDIR)/usb

#CFLAGS += -Map $(OUTPATH)/$(PROJ_NAME).map

OBJS = $(SRCS:.c=.o)
OBJS += $(USB_SRCS:.c=.o)
OBJS += $(S_SRCS:.s=.o)

# Make sure needed directories are there
_dummy := $(shell mkdir -p build/usb)

###################################################

.PHONY: lib proj

all: lib proj
	$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf

lib:
	$(MAKE) -C lib FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR)

proj: $(OUTPATH)/$(PROJ_NAME).elf

.s.o:
	$(AS) $(CPU) -o $(addprefix $(OUTPATH)/, $@) $<

.c.o:
	$(CC) $(CFLAGS) -std=gnu99 $(INCLUDE_PATHS) -o $(addprefix  $(OUTPATH)/, $@) $<

$(OUTPATH)/$(PROJ_NAME).elf: $(OBJS)
	$(LD) $(LDFLAGS) -T$(LINKER_SCRIPT) $(LIBPATHS) -o $@ $(addprefix $(OUTPATH)/, $^) $(LIBS) $(LD_SYS_LIBS)
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin
	$(OBJDUMP) -S --disassemble $(OUTPATH)/$(PROJ_NAME).elf > $(OUTPATH)/$(PROJ_NAME).dis

clean:
	rm -f $(OUTPATH)/*.o
	rm -f $(OUTPATH)/$(PROJ_NAME).elf
	rm -f $(OUTPATH)/$(PROJ_NAME).hex
	rm -f $(OUTPATH)/$(PROJ_NAME).bin
	rm -f $(OUTPATH)/$(PROJ_NAME).dis
	rm -f $(OUTPATH)/$(PROJ_NAME).map
	# Remove this line if you don't want to clean the libs as well
	$(MAKE) clean -C lib

