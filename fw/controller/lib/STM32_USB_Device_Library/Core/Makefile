BINPATH=

CC=$(BINPATH)arm-none-eabi-gcc
AR=$(BINPATH)arm-none-eabi-ar

vpath %.c src

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m0
CFLAGS += -msoft-float

CFLAGS += -ffreestanding -nostdlib

# Includes
CFLAGS += -Iinc -I$(BASEDIR) -I$(BASEDIR)/usb -I$(BASEDIR)/lib/CMSIS/Include -I$(BASEDIR)/lib/CMSIS/Device/ST/STM32F0xx/Include -I$(BASEDIR)/lib/STM32F0xx_StdPeriph_Driver/inc
CFLAGS += -I$(BASEDIR)/lib/STM32_USB_Device_Driver/inc
CFLAGS += -I$(BASEDIR)/lib/STM32_USB_Device_Library/Core/inc

# Default to STM32F072 if no device is passed
ifeq ($(DEVICE_DEF), )
DEVICE_DEF = STM32F072
endif

CFLAGS += -D$(DEVICE_DEF)

# Sources
SRCS = usbd_core.c usbd_ioreq.c usbd_req.c

OBJS = $(SRCS:.c=.o)
LIBNAME = libusbdevcore.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(LIBNAME): $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIBNAME)
