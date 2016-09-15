CCPREFIX ?= arm-none-eabi
CC = gcc

CFLAGS = -O0 
CFLAGS += -mfpu=vfp 
CFLAGS += -mfloat-abi=hard 
CFLAGS += -march=armv6zk 
CFLAGS += -g 
CFLAGS += -mtune=arm1176jzf-s 
CFLAGS += -nostartfiles

all: kernel.img

kernel.img: kernel.elf
	$(CCPREFIX)-objcopy kernel.elf -O binary kernel.img

kernel.elf: main.c
	$(CCPREFIX)-$(CC) $(CFLAGS) $^ -o $@

dump: kernel.elf
	$(CCPREFIX)-objdump -D kernel.elf > kernel.dump

clean:
	rm *.img *.elf
