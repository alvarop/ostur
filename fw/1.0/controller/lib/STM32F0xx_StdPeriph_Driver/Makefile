BINPATH=

CC=$(BINPATH)arm-none-eabi-gcc
AR=$(BINPATH)arm-none-eabi-ar

# Check for valid float argument
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = soft
endif
endif

vpath %.c src

CFLAGS  = -g -O2 -Wall -Werror
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m0
CFLAGS += -msoft-float

CFLAGS += -ffreestanding -nostdlib

# Includes
CFLAGS += -Iinc -I$(BASEDIR)/lib/CMSIS/Include -I$(BASEDIR)/lib/CMSIS/Device/ST/STM32F0xx/Include

# Default to STM32F072 if no device is passed
ifeq ($(DEVICE_DEF), )
DEVICE_DEF = STM32F072
endif

CFLAGS += -D$(DEVICE_DEF)

# Sources
SRCS = 	stm32f0xx_adc.c \
	stm32f0xx_can.c \
	stm32f0xx_cec.c \
	stm32f0xx_comp.c \
	stm32f0xx_crc.c \
	stm32f0xx_crs.c \
	stm32f0xx_dac.c \
	stm32f0xx_dbgmcu.c \
	stm32f0xx_dma.c \
	stm32f0xx_exti.c \
	stm32f0xx_flash.c \
	stm32f0xx_gpio.c \
	stm32f0xx_i2c.c \
	stm32f0xx_iwdg.c \
	stm32f0xx_misc.c \
	stm32f0xx_pwr.c \
	stm32f0xx_rcc.c \
	stm32f0xx_rtc.c \
	stm32f0xx_spi.c \
	stm32f0xx_syscfg.c \
	stm32f0xx_tim.c \
	stm32f0xx_usart.c \
	stm32f0xx_wwdg.c


OBJS = $(SRCS:.c=.o)
LIBNAME = libstdperiph.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(LIBNAME): $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIBNAME)
	